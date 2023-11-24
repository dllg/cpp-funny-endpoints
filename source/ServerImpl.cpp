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
    drogon::app().addListener("127.0.0.1", _port).run();
}

void ServerImpl::Stop() { drogon::app().quit(); }
