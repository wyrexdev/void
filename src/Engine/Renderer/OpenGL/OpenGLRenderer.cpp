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
    html->setText("");
    html->setBackgroundColor(glm::vec4(0, 0, 0, 0));
    html->start();
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

std::string OpenGLRenderer::parse(const std::string &content)
{
    auto tokens = tokenizer.tokenize(content);

    for (Entity *entity : elements)
    {
        delete entity;
    }
    elements.clear();

    std::string title = "Void";

    for (auto &t : tokens)
    {
        if (t.name == "title")
        {
            title = t.content;
        }
        else
        {
            // INLINE START
            if (t.name == "a")
            {
                Entity *entity = new Entity();
                entity->setText(t.content);
                entity->setBackgroundColor(glm::vec4(0, 0, 0, 0));
                entity->setBorderRadius(20);
                entity->setColor(glm::vec4(0.2f, 0.2f, 1, 1));
                entity->setType(ElementTypes::Block);
                elements.push_back(entity);
            }

            // BLOCK START
            if (t.name == "div")
            {
                Entity *entity = new Entity();
                entity->setText(t.content);
                entity->setBackgroundColor(glm::vec4(0, 0, 0, 0));
                entity->setType(ElementTypes::Block);
                elements.push_back(entity);
            }
        }
    }

    update();

    return title;
}