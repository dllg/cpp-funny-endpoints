#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "IClient.h"

namespace funny
{
using strfunc = std::function<std::string()>;

struct messagefunc
{
    std::string name;
    strfunc func;
    messagefunc(std::string n, strfunc f) : name(n), func(f) {}
};

class Funny
{
   public:
    Funny(std::shared_ptr<IClient> client);
    std::string Advice();
    std::string ChuckNorrisJoke();
    std::string DadJoke();
    std::string Message(size_t index);
    std::string Random();

   private:
    std::shared_ptr<IClient> _client;
    const std::vector<messagefunc> _messagefuncs = {
      {"Advice", [this]() { return Advice(); }},
      {"Chuck Norris Joke", [this]() { return ChuckNorrisJoke(); }},
      {"Dad Joke", [this]() { return DadJoke(); }},
    };
};
}  // namespace funny
