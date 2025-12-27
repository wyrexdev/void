#include "Headers/Global.hpp"

#include "QT/MainWindow.hpp"
#include "QT/Widget/Layouts/Nav.hpp"

#include "System/Setup/Setup.hpp"
#include "System/User/User.hpp"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/prctl.h>


#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cstdlib>


int main(int argc, char *argv[])
{
    umask(0077);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 5);
    format.setSamples(8);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    System::Setup setup;
    QT::MainWindow window;

    if (setup.isSetupNeeded())
    {
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            auto ids = System::User::getUserIds();
            setgid(ids.gid);
            setuid(ids.uid);

            prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
            setsid();
        }
    }

    std::cout << System::User::getCurrentUser() << std::endl;

    window.setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.background) + ";");
    window.setWindowTitle("Void Browser");
    window.showMaximized();

    return app.exec();
}

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

        Request req;
        read(client, &req, sizeof(req));

        Response res{};
        if (req.cmd == Command::READ_FILE)
        {
            res.status = readFile(req.path, res.data, &res.data_len);
        }

        write(client, &res, sizeof(res));
        close(client);
    }
}