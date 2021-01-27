#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include "Service.h"
#include "mocks/ClientMock.h"
#include "mocks/ServerMock.h"

using namespace funny;

using ::testing::Return;

class TestNotFound : public ::testing::TestWithParam<std::string>
{
};

std::vector<std::string> const TestNotFoundFormula = {
  "/some/strange/endpoint", "/", "/v2/advice", "/crocodile", ""
  // ...
};
TEST_P(TestNotFound, Methods)
{
    auto url = GetParam();
    auto client = std::make_shared<ClientMock>();
    auto server = std::make_shared<ServerMock>();
    auto service = Service(server, client);

    EXPECT_CALL(*server, Init()).WillOnce(Return(true));
    EXPECT_EQ(true, service.Init());
    EXPECT_CALL(*server, Run()).Times(1);
    service.Run();

    // Simulate http request to endpoint
    auto m = nlohmann::json::parse(server->call(url));

    EXPECT_EQ(true, m.contains("message"));
    EXPECT_EQ(true, m["message"].is_string());
    EXPECT_EQ(m["message"], "not found");
    EXPECT_EQ(true, m.contains("code"));
    EXPECT_EQ(true, m["code"].is_number_integer());
    EXPECT_EQ(m["code"], 404);
}

INSTANTIATE_TEST_SUITE_P(Service, TestNotFound, ::testing::ValuesIn(TestNotFoundFormula));
