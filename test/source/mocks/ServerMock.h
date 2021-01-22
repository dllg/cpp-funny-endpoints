#pragma once

#include <map>

#include "IServer.h"
#include "gmock/gmock.h"

namespace funny
{
using callback = std::function<std::string(void)>;

class ServerMock final : public IServer
{
   public:
    MOCK_METHOD(bool, Init, (), (override));
    virtual void Get(const std::string &endpoint, const callback &getCallback) override
    {
        _callbacks[endpoint] = getCallback;
    }
    MOCK_METHOD(void, Run, (), (override));
    MOCK_METHOD(void, Stop, (), (override));

    // Helper function to simulate a http request to an endpoint
    std::string call(const std::string &endpoint)
    {
        auto f = _callbacks.find(endpoint);
        if (f != _callbacks.end())
        {
            return f->second();
        }
        nlohmann::json j = {{"message", "Cannot find endpoint " + endpoint + "."}};
        return j.dump();
    }

   private:
    std::map<std::string, callback> _callbacks;
};
}  // namespace funny
