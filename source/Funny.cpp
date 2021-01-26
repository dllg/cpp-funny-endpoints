#include "Funny.h"

#include <nlohmann/json.hpp>
#include <random>

#include "IClient.h"
#include "spdlog/spdlog.h"

using namespace funny;

Funny::Funny(std::shared_ptr<IClient> client) : _client(client) {}

std::string Funny::Advice()
{
    std::string url = "https://api.adviceslip.com/advice";
    auto j = nlohmann::json::parse(_client->request(url, {}).c_str());
    if (j.contains("slip") && j["slip"].contains("advice") && j["slip"]["advice"].is_string())
    {
        nlohmann::json r = {{"message", j["slip"]["advice"].get<std::string>()}};
        return r.dump();
    }
    spdlog::error("Failed to receive advice from {}.", url);
    return "{}";
}

std::string Funny::ChuckNorrisJoke()
{
    std::string url = "https://api.chucknorris.io/jokes/random";
    auto j = nlohmann::json::parse(_client->request(url, {}).c_str());
    if (j.contains("value") && j["value"].is_string())
    {
        nlohmann::json r = {{"message", j["value"].get<std::string>()}};
        return r.dump();
    }
    spdlog::error("Failed to receive chuck norris joke from {}.", url);
    return "{}";
}

std::string Funny::DadJoke()
{
    std::string url = "https://icanhazdadjoke.com/";
    auto j = nlohmann::json::parse(_client->request(url, {{"Accept", "application/json"}}).c_str());
    if (j.contains("joke") && j["joke"].is_string())
    {
        nlohmann::json r = {{"message", j["joke"].get<std::string>()}};
        return r.dump();
    }
    spdlog::error("Failed to receive dad joke from {}.", url);
    return "{}";
}

std::string Funny::Message(size_t index)
{
    if (index < _messagefuncs.size())
    {
        nlohmann::json j = nlohmann::json::parse(_messagefuncs[index].func());
        nlohmann::json r = {{"message", _messagefuncs[index].name + ": " + j["message"].get<std::string>()}};

        return r.dump();
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
