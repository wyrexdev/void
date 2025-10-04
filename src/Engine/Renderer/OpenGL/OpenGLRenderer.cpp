#include "Engine/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"
#include "Engine/Renderer/OpenGL/Entities/Elements/Html.hpp"
#include "Engine/Renderer/OpenGL/Utils/Screen.hpp"

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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    html = new Html();
}

void OpenGLRenderer::resizeGL(int w, int h)
{
    ScreenUtils::width = w;
    ScreenUtils::height = h;

    glViewport(0, 0, w, h);
}

void OpenGLRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); 
        
    html->draw();

    for (Entity *entity : elements)
    {
        if (!entity->isInitalized)
        {
            entity->start();
            html->addEntity(entity);
        }

        entity->draw();
    }
}

void OpenGLRenderer::parse(const std::string &content)
{
    auto tokens = tokenizer.tokenize(content);

    for (Entity *entity : elements)
    {
        delete entity;
    }
    elements.clear();

    for (auto &t : tokens)
    {
        if (t.name != "script" && t.name != "style")
        {
            Entity *entity = new Entity();
            entity->setText(t.content);
            entity->setWidth(100);
            entity->setHeight(40);
            entity->setType(ElementTypes::Block);
            elements.push_back(entity);
        }
    }

    update();
}