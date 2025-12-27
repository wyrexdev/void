#include "Core/Sandbox/SandboxMain.hpp"

#include "Core/IPC/Server.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <pwd.h>
#include <cstdlib>
#include <iostream>

namespace IPC
{
    void SandboxMain::sandbox()
    {
        if (fork() == 0)
        {
            execlp(
                "pkexec",
                "pkexec",
                "./Void",
                "--sandbox",
                nullptr);
            _exit(1);
        }
    }
} // namespace IPC
