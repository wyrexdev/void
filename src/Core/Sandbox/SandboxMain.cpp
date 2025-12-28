#include "Core/Sandbox/SandboxMain.hpp"

namespace Sandbox
{
    void SandboxMain::sandbox()
    {
        execlp(
            "pkexec",
            "pkexec",
            "./Void",
            "--ipc-server-sandbox",
            nullptr);
        _exit(1);
    }
} // namespace IPC
