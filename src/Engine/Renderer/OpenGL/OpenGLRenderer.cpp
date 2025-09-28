#include "Engine/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Engine/Renderer/OpenGL/Entities/Entity.hpp"
#include "Engine/Renderer/OpenGL/Entities/Elements/Html.hpp"
#include "Engine/Renderer/OpenGL/Utils/Screen.hpp"

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

    for (Entity *entity : elements)
    {
        if (!entity->isInitalized)
        {
            entity->start();
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
        Entity *entity = nullptr;
        Html *html = new Html();

        switch (t.type)
        {
        case TokenType::StartTag:
            std::cout << t.name << std::endl;
            entity = new Entity();
            entity->setType(ElementTypes::Inline);
            entity->setText(t.name);
            html->addEntity(entity);
            break;
        }

        elements.push_back(html);

        if (entity)
            elements.push_back(entity);
    }

    update();
}