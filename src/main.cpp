#include "Headers/Global.hpp"

#include <sys/prctl.h>

#include "QT/MainWindow.hpp"
#include "QT/Widget/Layouts/Nav.hpp"

#include "System/Setup/Setup.hpp"
#include "System/User/User.hpp"

#include "Core/Sandbox/SandboxMain.hpp"

#include "Core/IPC/Client.hpp"

int main(int argc, char *argv[])
{
    umask(0077);

    if (argc > 1 && strcmp(argv[1], "--sandbox") == 0)
    {
        auto ids = System::User::getUserIds();

        initgroups("void", ids.gid);
        setgid(ids.gid);
        setuid(ids.uid);

        prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);

        setsid();

        IPC::Server *server = new IPC::Server();
        server->initServer();

        IPC::Client *client = new IPC::Client();
        client->initClient();
        client->sendRequest();
        
        _exit(0);

        return 0;
    }

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 5);
    format.setSamples(8);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    System::Setup *setup = new System::Setup();
    QT::MainWindow *window = new QT::MainWindow();
    IPC::SandboxMain *sm = new IPC::SandboxMain();

    window->setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.background) + ";");
    window->setWindowTitle("Void Browser");
    window->showMaximized();

    return app.exec();
}