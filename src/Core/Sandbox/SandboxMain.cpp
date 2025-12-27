#include "Core/Sandbox/SandboxMain.hpp"

namespace IPC
{
    void SandboxMain::sandbox()
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

            if (req.cmd == Command::WRITE_FILE)
            {
                res.status = FileStore::writeFile(
                    req.path,
                    req.data,
                    req.data_len);

                res.data_len = 0;
            }

            write(client, &res, sizeof(res));
            close(client);
        }
    }
} // namespace IPC
