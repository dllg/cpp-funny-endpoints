#pragma once

#include <map>
#include <string>

namespace funny
{
class IClient
{
   public:
    virtual const std::string &request(const std::string &url,
                                       const std::map<std::string, std::string> &headers) = 0;
};
}  // namespace funny
