#include "Headers/Global.hpp"

#include "QT/MainWindow.hpp"
#include "QT/Widget/Layouts/Nav.hpp"

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 5);
    format.setSamples(8);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    QT::MainWindow *window = new QT::MainWindow();
    window->setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.background) + ";");
    window->setWindowTitle("Void Browser");
    window->showMaximized();
    
    return app.exec();
}