#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility {
    Q_OBJECT

public:
    OpenGLRenderer(QWidget *parent = nullptr);
    ~OpenGLRenderer();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};