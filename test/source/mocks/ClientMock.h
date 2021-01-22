#pragma once

#include "IClient.h"
#include "gmock/gmock.h"

namespace funny
{
class ClientMock final : public IClient
{
   public:
    MOCK_METHOD((const std::string &), request,
                (const std::string &url, (const std::map<std::string, std::string> &headers)), (override));
};
}  // namespace funny
