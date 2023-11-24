#pragma once

#include "IServer.h"

namespace funny
{
class ServerImpl final : public IServer
{
   public:
    ServerImpl(std::string host, int port) : _host(host), _port(port) {}
    virtual ~ServerImpl() {}
    virtual bool Init() override;
    virtual void Get(const std::string &endpoint, const callback &getCallback) override;
    virtual void Run() override;
    virtual void Stop() override;

   private:
    std::string _host;
    int _port;
    std::vector<callback> _getCallbacks;
};
}  // namespace funny
