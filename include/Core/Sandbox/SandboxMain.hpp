#pragma once

#include "Core/IPC/Server.hpp"
#include "Utils/File/FS.hpp"

#include <grp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <pwd.h>
#include <cstdlib>
#include <iostream>
#include <signal.h>

namespace Sandbox
{
    class SandboxMain
    {
    public:
        void sandbox();
    };
} // namespace IPC