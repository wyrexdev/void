#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Compatibility>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <vector>

#include "Engine/Parser/Html/Tokenizer/Tokenizer.hpp"

class Entity;

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility {
    Q_OBJECT

public:
    OpenGLRenderer(QWidget *parent = nullptr);
    ~OpenGLRenderer();

    std::string parse(const std::string &content);

    Tokenizer tokenizer;

private:
    std::vector<Entity*> elements;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};