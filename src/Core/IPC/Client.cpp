#include "Core/IPC/Client.hpp"

namespace IPC
{
    void Client::initClient()
    {
        sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("socket");
            return;
        }

        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, "/tmp/void.sock");

        if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("connect");
            close(sock);
            sock = -1;
        }
    }

    Protocol::Response Client::sendRequest()
    {
        Protocol::Request req{};
        req.magic = 0xDEADBEEF;
        req.cmd = Command::WRITE_FILE;
        strcpy(req.path, "hello.txt");

        const char msg[] = "Merhaba Void Sandbox";
        req.data_len = sizeof(msg);
        memcpy(req.data, msg, req.data_len);

        write(sock, &req, sizeof(req));

        Protocol::Response res{};
        read(sock, &res, sizeof(res));

        close(sock);

        return res;
    }
} // namespace IPC
