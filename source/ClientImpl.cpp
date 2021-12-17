#include "ClientImpl.h"

#include <httplib/httplib.h>

#include <regex>

#include "spdlog/spdlog.h"

using namespace funny;

// getHostAndEndpoint will get the host and path part of an url
bool getHostAndEndpoint(const std::string &url, std::string &host, std::string &path)
{
    std::size_t pos = 0;
    for (int i = 0; i < 3; ++i)
    {
        pos = url.find('/', pos + 1);
        if (pos == std::string::npos)
        {
            return false;
        }
    }
    host = url.substr(0, pos);
    path = url.substr(pos);
    return true;
}

const std::string &ClientImpl::request(const std::string &url,
                                       const std::map<std::string, std::string> &headers)
{
    spdlog::info("Sending request to {}.", url);
    _response = "";
    auto h = httplib::Headers(headers.begin(), headers.end());

    std::string host, path;
    if (!getHostAndEndpoint(url, host, path))
    {
        spdlog::error("Failed parsing the url '{}'.", url);
        return _response;
    }

    httplib::Client c(host);
    httplib::Result r = c.Get(path.c_str(), h);
    if (r.error() != httplib::Error::Success)
    {
        spdlog::error("Received error {} after requesting to '{}'.", httplib::to_string(r.error()), url);
        return _response;
    }
    _response = r.value().body;
    return _response;
}
