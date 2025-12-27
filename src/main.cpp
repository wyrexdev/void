#include "Headers/Global.hpp"

#include <sys/prctl.h>

#include "QT/MainWindow.hpp"
#include "QT/Widget/Layouts/Nav.hpp"

#include "System/Setup/Setup.hpp"
#include "System/User/User.hpp"

#include "Core/Sandbox/SandboxMain.hpp"

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

    System::Setup *setup = new System::Setup();
    QT::MainWindow *window = new QT::MainWindow();
    IPC::SandboxMain *sm = new IPC::SandboxMain();

    if (setup->isSetupNeeded())
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

            sm->sandbox();
            _exit(0);
        }
    }

    std::cout << System::User::getCurrentUser() << std::endl;

    window->setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.background) + ";");
    window->setWindowTitle("Void Browser");
    window->showMaximized();

    return app.exec();
}