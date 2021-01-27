#include "ClientImpl.h"

#include <cpr/cpr.h>

#include <regex>

#include "spdlog/spdlog.h"

using namespace funny;

const std::string &ClientImpl::request(const std::string &url,
                                       const std::map<std::string, std::string> &headers)
{
    spdlog::info("Sending request to {}.", url);
    _response = "{}";
    auto h = cpr::Header();
    for (auto &&hh : headers)
    {
        h[hh.first] = hh.second;
    }
    cpr::Response r = cpr::Get(cpr::Url{url}, h);
    if (r.status_code != 200)
    {
        spdlog::error("Received {} from after requesting to '{}'.", r.status_code, url);
        return _response;
    }
    _response = r.text;
    return _response;
}
