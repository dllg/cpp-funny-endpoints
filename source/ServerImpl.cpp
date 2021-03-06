#include "ServerImpl.h"

#include "spdlog/spdlog.h"

using namespace funny;

bool ServerImpl::Init()
{
    _app.reset(new uWS::App());
    if (_app->constructorFailed())
    {
        return false;
    }

    _app->any("/*", [](auto *res, auto *req) {
        (void)req;
        res->cork([res]() {
            res->writeStatus("404 NOT FOUND")
              ->writeHeader("Content-Type", "application/json")
              ->end("{\"message\":\"not found\",\"code\":404}");
        });
    });
    return true;
}

void ServerImpl::Get(const std::string &endpoint, const callback &getCallback)
{
    _getCallbacks.push_back(getCallback);
    const auto &getcall = _getCallbacks.back();
    _app->get(endpoint, [getcall](auto *res, auto *req) {
        (void)req;
        res->cork([res, getcall]() { res->writeHeader("Content-Type", "application/json")->end(getcall()); });
    });
}

void ServerImpl::Run()
{
    _app
      ->listen(_port,
               [this](auto *listenSocket) {
                   if (listenSocket)
                   {
                       _listenSocket = listenSocket;
                       spdlog::info("Starting server. Listening on port {}.", _port);
                   }
               })
      .run();
}

void ServerImpl::Stop() { us_listen_socket_close(0, _listenSocket); }
