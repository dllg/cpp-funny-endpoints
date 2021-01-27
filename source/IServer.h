#pragma once

#include <functional>
#include <string>

namespace funny
{
using callback = std::function<std::string(void)>;

class IServer
{
   public:
    virtual bool Init() = 0;
    virtual void Get(const std::string &endpoint, const callback &getCallback) = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;
};
}  // namespace funny
