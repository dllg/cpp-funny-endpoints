#include <argh.h>

#include <csignal>
#include <thread>

#include "ClientImpl.h"
#include "ServerImpl.h"
#include "Service.h"
#include "spdlog/spdlog.h"

using namespace funny;

std::shared_ptr<IServer> server;
void signalHandler(int s)
{
    (void)s;
    spdlog::info("Exiting.");
    if (server) server->Stop();
}

int main(int argc, char** argv)
{
    auto cmdl = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
    if (cmdl[{"-h", "--help", "-?"}])
    {
        spdlog::info("Usage: {} --host <host> --port <port>", cmdl[0]);
        return 0;
    }

    std::string host;
    cmdl({"-h", "--host"}, "127.0.0.1") >> host;

    int port;
    cmdl({"-p", "--port"}, 5000) >> port;

    auto client = std::make_shared<ClientImpl>();
    server = std::make_shared<ServerImpl>(host, port);
    auto service = Service(server, client);

    if (!service.Init())
    {
        spdlog::error("{}", service.Error());
        return 1;
    }

    signal(SIGINT, signalHandler);

    service.Run();
    return 0;
}
