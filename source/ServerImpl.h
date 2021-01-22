#pragma once

#include <libusockets.h>

#include "IServer.h"
#include "libusockets.h"
#include "uWebSockets/App.h"

namespace funny
{
class ServerImpl final : public IServer
{
   public:
    ServerImpl(int port) : _port(port) {}
    virtual ~ServerImpl() {}
    virtual bool Init() override;
    virtual void Get(const std::string &endpoint, const callback &getCallback) override;
    virtual void Run() override;
    virtual void Stop() override;

   private:
    int _port;
    std::shared_ptr<uWS::App> _app;
    std::vector<callback> _getCallbacks;
    struct us_listen_socket_t *_listenSocket;
};
}  // namespace funny
