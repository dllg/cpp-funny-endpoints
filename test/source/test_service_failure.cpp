#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "Service.h"
#include "mocks/ClientMock.h"
#include "mocks/ServerMock.h"

using namespace funny;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

struct ServiceTestFailStruct
{
    std::string endpoint;
    std::string reply;
    std::function<void(std::shared_ptr<ClientMock>, const std::string &)> mockCall;
    std::string want;

    ServiceTestFailStruct(std::string e, std::string r,
                          std::function<void(std::shared_ptr<ClientMock>, const std::string &)> c,
                          std::string w)
        : endpoint(e), reply(r), mockCall(c), want(w)
    {
    }
};

class ServiceTestFail : public ::testing::TestWithParam<ServiceTestFailStruct>
{
};

std::vector<ServiceTestFailStruct> const ServiceTestFailFormula = {
  // Advice - no json response
  {"/v1/advice", "Always get two ciders.",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.adviceslip.com/advice";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillOnce(ReturnRef(reply));
   },
   "request failed to https://api.adviceslip.com/advice."},
  // Advice - strange json response
  {"/v1/advice", "{\"some\":\"response\"}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.adviceslip.com/advice";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillOnce(ReturnRef(reply));
   },
   "request failed to https://api.adviceslip.com/advice."},
  // Chuck Norris Joke - no json, empty response
  {"/v1/chucknorris", "",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.chucknorris.io/jokes/random";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillRepeatedly(ReturnRef(reply));
   },
   "request failed to https://api.chucknorris.io/jokes/random."},
  // Chuck Norris Joke - strange json
  {"/v1/chucknorris", "{\"some\":\"bad json\"}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.chucknorris.io/jokes/random";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillRepeatedly(ReturnRef(reply));
   },
   "request failed to https://api.chucknorris.io/jokes/random."},
  // Dad Joke - text response, no json
  {"/v1/dadjoke", "What did one nut say as he chased another nut?  I'm a cashew!",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://icanhazdadjoke.com/";
       std::map<std::string, std::string> headers = {{"Accept", "application/json"}};
       EXPECT_CALL(*mock, request(url, headers)).WillRepeatedly(ReturnRef(reply));
   },
   "request failed to https://icanhazdadjoke.com/."},
  // Dad Joke - strange json
  {"/v1/dadjoke", "{\"number\": 222}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://icanhazdadjoke.com/";
       std::map<std::string, std::string> headers = {{"Accept", "application/json"}};
       EXPECT_CALL(*mock, request(url, headers)).WillRepeatedly(ReturnRef(reply));
   },
   "request failed to https://icanhazdadjoke.com/."},
  // Random - text response, no json
  {"/v1/random", "some random text reponse",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       EXPECT_CALL(*mock, request(_, _)).WillRepeatedly(ReturnRef(reply));
   },
   "request failed to "},
  // Random - strange json response
  {"/v1/random", "{\"percentage\": 0.55}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       EXPECT_CALL(*mock, request(_, _)).WillRepeatedly(ReturnRef(reply));
   },
   "request failed to "},
  // ...
};
TEST_P(ServiceTestFail, Methods)
{
    auto p = GetParam();
    auto client = std::make_shared<ClientMock>();
    auto server = std::make_shared<ServerMock>();
    auto service = Service(server, client);

    EXPECT_CALL(*server, Init()).WillOnce(Return(true));
    EXPECT_EQ(true, service.Init());
    EXPECT_CALL(*server, Run()).Times(1);
    service.Run();

    auto r = p.reply;
    p.mockCall(client, r);

    // Simulate http request to endpoint
    auto m = nlohmann::json::parse(server->call(p.endpoint));

    EXPECT_EQ(true, m.contains("error"));

    if (p.endpoint == "/v1/random")
    {
        std::string got = m["error"];
        EXPECT_TRUE(got.find(p.want) != std::string::npos);
    }
    else
    {
        EXPECT_EQ(m["error"], p.want);
    }
}

INSTANTIATE_TEST_SUITE_P(Service, ServiceTestFail, ::testing::ValuesIn(ServiceTestFailFormula));
