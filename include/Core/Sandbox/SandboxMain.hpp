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
    class SandboxMain
    {
        void sandboxMain()
        {
            int server = socket(AF_UNIX, SOCK_STREAM, 0);

            sockaddr_un addr{};
            addr.sun_family = AF_UNIX;
            strcpy(addr.sun_path, "/var/run/void.sock");

            unlink(addr.sun_path);
            bind(server, (sockaddr *)&addr, sizeof(addr));
            listen(server, 5);

            while (true)
            {
                int client = accept(server, nullptr, nullptr);

                Protocol::Request req;
                read(client, &req, sizeof(req));

                Protocol::Response res{};
                if (req.cmd == Command::READ_FILE)
                {
                    res.status = FileStore::readFile(req.path, res.data, &res.data_len);
                }

                write(client, &res, sizeof(res));
                close(client);
            }
        }
    };
} // namespace IPC