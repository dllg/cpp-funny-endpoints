#include <csignal>
#include <thread>

#include "ClientImpl.h"
#include "ServerImpl.h"
#include "Service.h"
#include "spdlog/spdlog.h"

using namespace funny;

auto client = std::make_shared<ClientImpl>();
auto server = std::make_shared<ServerImpl>(5000);
auto service = Service(server, client);
void signalHandler(int s)
{
    (void)s;
    spdlog::info("Exiting.");
    server->Stop();
}

int main(int argc, char** argv)
{
    if (!service.Init())
    {
        spdlog::error("{}", service.Error());
        return 1;
    }

    signal(SIGINT, signalHandler);

    service.Run();
    return 0;
}
