#include "Core/IPC/Server.hpp"

namespace IPC
{
    void Server::initServer()
    {
        int server = socket(AF_UNIX, SOCK_STREAM, 0);
        if (server < 0)
        {
            perror("socket");
            return;
        }

        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, "/tmp/void.sock");

        unlink(addr.sun_path);

        if (bind(server, (sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("bind");
            close(server);
            return;
        }

        chmod("/tmp/void.sock", 0666);

        if (listen(server, 5) < 0)
        {
            perror("listen");
            close(server);
            return;
        }

        while (true)
        {
            int client = accept(server, nullptr, nullptr);
            if (client < 0)
            {
                perror("accept");
                continue;
            }

            Protocol::Request req{};
            if (recv(client, &req, sizeof(req), MSG_WAITALL) != sizeof(req))
            {
                close(client);
                continue;
            }

            Protocol::Response res{};
            if (req.cmd == Command::READ_FILE)
            {
                res.status = FileStore::readFile(req.path, res.data, &res.data_len);
            }
            else if (req.cmd == Command::WRITE_FILE)
            {
                res.status = FileStore::writeFile(req.path, req.data, req.data_len);
                res.data_len = 0;
            } else if (req.cmd == Command::READ_COOKIE) {

            } else if (req.cmd == Command::WRITE_COOKIE) {
                
            }

            write(client, &res, sizeof(res));
            close(client);
        }
    }

} // namespace IPC
