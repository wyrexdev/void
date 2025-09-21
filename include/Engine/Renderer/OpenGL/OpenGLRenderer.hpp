#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLRenderer(QWidget *parent = nullptr);
    ~OpenGLRenderer();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};