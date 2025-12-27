#include "Core/IPC/Client.hpp"

namespace IPC
{
    void Client::initClient()
    {
        int sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("socket");
        }

        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, "/var/run/void.sock");

        if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("connect");
        }
    }

    Protocol::Response Client::sendRequest(Protocol::Request req)
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

        std::cout << "Server status: " << res.status << std::endl;

        close(sock);

        return res;
    }
} // namespace IPC
