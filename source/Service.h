#pragma once

#include <functional>
#include <memory>

#include "Funny.h"
#include "IClient.h"
#include "IServer.h"

namespace funny
{
class Service
{
   public:
    Service(const std::shared_ptr<IServer> &server, const std::shared_ptr<IClient> &client);
    bool Init();
    void Run();
    const std::string &Error();

   private:
    std::shared_ptr<IServer> _server;
    Funny _funny;
    bool _isInitialized;
    std::string _error;
};
}  // namespace funny