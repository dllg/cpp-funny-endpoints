#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "Funny.h"
#include "mocks/ClientMock.h"
#include "mocks/ServerMock.h"

using namespace funny;

using ::testing::_;
using ::testing::ReturnRef;

struct FunnyTestAPIStruct
{
    int index;
    std::string reply;
    std::function<void(std::shared_ptr<ClientMock>, const std::string &)> mockCall;
    std::string want;

    FunnyTestAPIStruct(int i, std::string r,
                       std::function<void(std::shared_ptr<ClientMock>, const std::string &)> c, std::string w)
        : index(i), reply(r), mockCall(c), want(w)
    {
    }
};

class FunnyAPITest : public ::testing::TestWithParam<FunnyTestAPIStruct>
{
};

std::vector<FunnyTestAPIStruct> const FunnyAPITestFormula = {
  // Advice
  {0, "{\"slip\": { \"id\": 183, \"advice\": \"Always get two ciders.\"}}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.adviceslip.com/advice";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillOnce(ReturnRef(reply));
   },
   "Advice: Always get two ciders."},
  // Chuck Norris Joke
  {1,
   "{\"categories\":[],\"created_at\":\"2020-01-05 "
   "13:42:24.696555\",\"icon_url\":\"https://assets.chucknorris.host/img/avatar/"
   "chuck-norris.png\",\"id\":\"3Odt13-SQ06Pq-RUPwRQ4w\",\"updated_at\":\"2020-01-05 "
   "13:42:24.696555\",\"url\":\"https://api.chucknorris.io/jokes/3Odt13-SQ06Pq-RUPwRQ4w\",\"value\":\"Peace "
   "disturbs Chuck Norris.\"}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://api.chucknorris.io/jokes/random";
       std::map<std::string, std::string> headers;
       EXPECT_CALL(*mock, request(url, headers)).WillOnce(ReturnRef(reply));
   },
   "Chuck Norris Joke: Peace disturbs Chuck Norris."},
  // Dad Joke
  {2,
   "{\"id\":\"0DQKB51oGlb\",\"joke\":\"What did one nut say as he chased another nut?  I'm a "
   "cashew!\",\"status\":200}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       std::string url = "https://icanhazdadjoke.com/";
       std::map<std::string, std::string> headers = {{"Accept", "application/json"}};
       EXPECT_CALL(*mock, request(url, headers)).WillOnce(ReturnRef(reply));
   },
   "Dad Joke: What did one nut say as he chased another nut?  I'm a cashew!"},
  // Bad index
  {3, "{\"some\":\"value\"}",
   [](std::shared_ptr<ClientMock> mock, const std::string &reply) {
       EXPECT_CALL(*mock, request(_, _)).Times(0);
   },
   "Cannot find message with index 3."},
  // ...
};
TEST_P(FunnyAPITest, Methods)
{
    auto p = GetParam();
    auto client = std::make_shared<ClientMock>();
    auto funny = Funny(client);

    auto r = p.reply;
    p.mockCall(client, r);

    auto m = nlohmann::json::parse(funny.Message(p.index));

    EXPECT_EQ(true, m.contains("message"));
    EXPECT_EQ(m["message"], p.want);
}

INSTANTIATE_TEST_SUITE_P(Funny, FunnyAPITest, ::testing::ValuesIn(FunnyAPITestFormula));
