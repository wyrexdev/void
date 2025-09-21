#include "Engine/Renderer/OpenGL/OpenGLRenderer.hpp"

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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLRenderer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.8f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(-0.8f, -0.8f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.8f, -0.8f);
    glEnd();
}