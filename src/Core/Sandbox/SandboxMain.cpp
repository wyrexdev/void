#include "Core/Sandbox/SandboxMain.hpp"

namespace IPC
{
    void SandboxMain::sandbox()
    {
        Server *server = new Server();

        server->initServer();
    }
} // namespace IPC
