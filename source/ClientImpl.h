#pragma once

#include "IClient.h"

namespace funny
{
class ClientImpl final : public IClient
{
   public:
    virtual ~ClientImpl() {}
    virtual const std::string &request(const std::string &url,
                                       const std::map<std::string, std::string> &headers) override;

   private:
    std::string _response;
};
}  // namespace funny
