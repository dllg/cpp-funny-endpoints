#include "Funny.h"

#include <nlohmann/json.hpp>
#include <random>

#include "IClient.h"
#include "spdlog/spdlog.h"

using namespace funny;

Funny::Funny(std::shared_ptr<IClient> client) : _client(client) {}

std::string Funny::Advice()
{
    return sendApiRequest("https://api.adviceslip.com/advice", {}, {"slip", "advice"});
}

std::string Funny::ChuckNorrisJoke()
{
    return sendApiRequest("https://api.chucknorris.io/jokes/random", {}, {"value"});
}

std::string Funny::DadJoke()
{
    return sendApiRequest("https://icanhazdadjoke.com/", {{"Accept", "application/json"}}, {"joke"});
}

std::string Funny::Message(size_t index)
{
    if (index < _messagefuncs.size())
    {
        nlohmann::json j = nlohmann::json::parse(_messagefuncs[index].func());
        if (j.contains("message"))
        {
            nlohmann::json r = {
              {"message", _messagefuncs[index].name + ": " + j["message"].get<std::string>()}};
            return r.dump();
        }
        return j.dump();
    }
    nlohmann::json r = {{"message", "Cannot find message with index " + std::to_string(index) + "."}};
    return r.dump();
}

std::string Funny::Random()
{
    // generate random index
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> distribution(0, _messagefuncs.size() - 1);

    return Message(distribution(e));
}

std::string Funny::sendApiRequest(const std::string &url, const std::map<std::string, std::string> &headers,
                                  const std::vector<std::string> &keys)
{
    try
    {
        auto j = nlohmann::json::parse(_client->request(url, headers).c_str());
        auto jj = j;
        bool ok = true;
        for (auto &&key : keys)
        {
            ok = ok && jj.contains(key) && (jj[key].is_object() || jj[key].is_string());
            jj = jj[key];
        }
        if (ok)
        {
            nlohmann::json r = {{"message", jj.get<std::string>()}};
            return r.dump();
        }
    }
    catch (nlohmann::json::parse_error &)
    {
    }
    std::string errmsg = "request failed to " + url + ".";
    spdlog::error(errmsg);
    nlohmann::json r = {{"error", errmsg}};
    return r.dump();
}
