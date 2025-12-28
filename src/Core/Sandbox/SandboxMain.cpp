#include "Core/Sandbox/SandboxMain.hpp"

namespace IPC
{
    void SandboxMain::sandbox()
    {
        execlp(
            "ls"
            "./Void",
            "--ipc-server-sandbox",
            nullptr);
        _exit(1);
    }
} // namespace IPC
