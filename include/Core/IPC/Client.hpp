#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "Core/IPC/Protocol.hpp"

namespace IPC
{
    class Client
    {
    public:
        void initClient();
        Protocol::Response sendRequest(Protocol::Request req);

    private:
        int sock;
    };
} // namespace IPC