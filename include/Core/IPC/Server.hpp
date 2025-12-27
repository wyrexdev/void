#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include "Core/IPC/Protocol.hpp"
#include "Core/Storage/FileStore.hpp"

namespace IPC
{
    class Server
    {
    public:
        void initServer();
    };
} // namespace IPC