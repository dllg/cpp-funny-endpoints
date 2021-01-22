#include "Service.h"

using namespace funny;

Service::Service(const std::shared_ptr<IServer> &server, const std::shared_ptr<IClient> &client)
    : _server(server), _funny(client), _isInitialized(false), _error()
{
}

bool Service::Init()
{
    if (_server)
    {
        _isInitialized = _server->Init();
    }
    if (!_isInitialized)
    {
        _error = "Failed to initialized server";
        return false;
    }
    _server->Get("/v1/advice", [this]() { return _funny.Advice(); });
    _server->Get("/v1/chucknorris", [this]() { return _funny.ChuckNorrisJoke(); });
    _server->Get("/v1/dadjoke", [this]() { return _funny.DadJoke(); });
    _server->Get("/v1/random", [this]() { return _funny.Random(); });
    return true;
}

void Service::Run()
{
    if (_isInitialized)
    {
        _server->Run();
    }
}

const std::string &Service::Error() { return _error; }
