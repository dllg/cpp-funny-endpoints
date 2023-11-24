#include "ServerImpl.h"

#include "spdlog/spdlog.h"

#include <drogon/drogon.h>

using namespace funny;

bool ServerImpl::Init()
{
    return true;
}

void ServerImpl::Get(const std::string &endpoint, const callback &getCallback)
{
    _getCallbacks.push_back(getCallback);
    const auto &getcall = _getCallbacks.back();
    drogon::app().registerHandler(
        endpoint,
        [getcall](const drogon::HttpRequestPtr &,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody(getcall());
            callback(resp);
        },
        {drogon::Get});
}

void ServerImpl::Run()
{
    spdlog::info("Starting server on {} using port {}.", _host, _port);
    drogon::app().addListener(_host, _port).run();
    spdlog::info("Server stopped.");
}

void ServerImpl::Stop() { drogon::app().quit(); }
