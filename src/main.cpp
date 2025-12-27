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