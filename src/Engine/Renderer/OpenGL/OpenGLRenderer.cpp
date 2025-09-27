#include "Engine/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"
#include "Engine/Renderer/OpenGL/Entities/Elements/Html.hpp"
#include "Engine/Renderer/OpenGL/Utils/Screen.hpp"

Entity *t;

Html *html;

OpenGLRenderer::OpenGLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    html = new Html();
    html->setText("");
    html->setPadding(10, 10, 100, 10);
    html->setMargin(10, 10, 20, 50);
    html->start();

    t = new Entity();
    t->setWidth(150);
    t->setHeight(150);
    t->setBorderRadius(40.0f);
    t->enableRoundedCorners(true);
    t->start();

    html->addEntity(t);
}

void OpenGLRenderer::resizeGL(int w, int h)
{
    ScreenUtils::width = w; ScreenUtils::height = h;

    glViewport(0, 0, w, h);
}

void OpenGLRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    html->draw();

    t->draw();
}