#include "Engine/Engine.hpp"

Engine::Engine()
{
}

std::string Engine::parse(const std::string &content)
{
    this->content = content;
    return "";
}

SkiaRenderer *Engine::getSkiaView()
{
    return this;
}

void Engine::onInit()
{
}

void Engine::onRender()
{
    t = new Skia::TextRenderer(canvas);
    t->init();
    t->render();
    t->setText(content);
}

void Engine::onResize(int w, int h)
{
    Screen::width = w;
    Screen::height = h;
}