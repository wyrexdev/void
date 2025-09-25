#include "Engine/Renderer/OpenGL/OpenGLRenderer.hpp"

#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"

Entity *t;

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

    t = new Entity();
    t->setPosition(100, 100, 0);
    t->setWidth(150);
    t->setHeight(50);
    t->start();
}

void OpenGLRenderer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    t->draw();
}