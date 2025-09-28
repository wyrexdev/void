#include "Engine/Engine.hpp"

Engine::Engine()
{
}

void Engine::init(Widget *siteContentWidget)
{
    openglWindow = new OpenGLRenderer(siteContentWidget);
}

void Engine::setRenderEngine(int type)
{
    this->type = type;
}

void Engine::addRenderLayout(QVBoxLayout *siteContentLayout)
{
    if (type == EngineTypes::OpenGL)
    {
        siteContentLayout->addWidget(openglWindow);
    }
    else if (type == EngineTypes::Vulkan)
    {
    }
}

void Engine::parse(const std::string &content)
{
    if (type == EngineTypes::OpenGL)
    {
        openglWindow->parse(content);
    }
    else if (type == EngineTypes::Vulkan)
    {
    }
}