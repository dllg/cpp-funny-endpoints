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

struct ServiceTestAPIStruct
{
    std::string endpoint;
    std::string reply;
    std::function<void(std::shared_ptr<ClientMock>, const std::string &)> mockCall;
    std::string want;

    ServiceTestAPIStruct(std::string e, std::string r,
                         std::function<void(std::shared_ptr<ClientMock>, const std::string &)> c,
                         std::string w)
        : endpoint(e), reply(r), mockCall(c), want(w)
    {
    }
};

class ServiceAPITest : public ::testing::TestWithParam<ServiceTestAPIStruct>
{
};

std::vector<ServiceTestAPIStruct> const ServiceAPITestFormula = {
  // Advice
  {"/v1/advice", "{\"slip\": { \"id\": 183, \"advice\": \"Always get two ciders.\"}}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.adviceslip.com/advice";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillOnce(ReturnRef(reply));
   },
   "Always get two ciders."},
  // Chuck Norris Joke
  {"/v1/chucknorris",
   "{\"categories\":[],\"created_at\":\"2020-01-05 "
   "13:42:24.696555\",\"icon_url\":\"https://assets.chucknorris.host/img/avatar/"
   "chuck-norris.png\",\"id\":\"3Odt13-SQ06Pq-RUPwRQ4w\",\"updated_at\":\"2020-01-05 "
   "13:42:24.696555\",\"url\":\"https://api.chucknorris.io/jokes/3Odt13-SQ06Pq-RUPwRQ4w\",\"value\":\"Peace "
   "disturbs Chuck Norris.\"}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.chucknorris.io/jokes/random";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillRepeatedly(ReturnRef(reply));
   },
   "Peace disturbs Chuck Norris."},
  // Dad Joke
  {"/v1/dadjoke",
   "{\"id\":\"0DQKB51oGlb\",\"joke\":\"What did one nut say as he chased another nut?  I'm a "
   "cashew!\",\"status\":200}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://icanhazdadjoke.com/";
       std::map<std::string, std::string> headers = {{"Accept", "application/json"}};
       EXPECT_CALL(*mock, request(url, headers)).WillRepeatedly(ReturnRef(reply));
   },
   "What did one nut say as he chased another nut?  I'm a cashew!"},
  // Random
  {"/v1/random",
   "{\"slip\": {\"advice\": \"random endpoint result\"},\"value\": \"random endpoint result\",\"joke\": "
   "\"random endpoint result\"}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       EXPECT_CALL(*mock, request(_, _)).WillRepeatedly(ReturnRef(reply));
   },
   ": random endpoint result"},
  // Bad endpoint
  {"/bad/endpoint", "{\"some\":\"value\"}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       EXPECT_CALL(*mock, request(_, _)).Times(0);
   },
   "not found"},
  // ...
};
TEST_P(ServiceAPITest, Methods)
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

    EXPECT_EQ(true, m.contains("message"));

    if (p.endpoint == "/v1/random")
    {
        std::string got = m["message"];
        EXPECT_TRUE(got.find(p.want) != std::string::npos);
    }
    else
    {
        EXPECT_EQ(m["message"], p.want);
    }
}

INSTANTIATE_TEST_SUITE_P(Service, ServiceAPITest, ::testing::ValuesIn(ServiceAPITestFormula));
