#include "Engine/Engine.hpp"

Engine::Engine()
{
}

std::string Engine::parse(const std::string &content)
{
    this->content = content;

    Tokenizer *t = new Tokenizer();

    std::string title = "Unknown - Void";

    tokens = t->tokenize(content);

    for (Token token : tokens)
    {
        if (token.name == "title")
        {
            title = token.content;
            continue;
        }
    }

    for (Token t : tokens)
    {
        if (t.name == "a")
        {
            Element element;

            element.name = t.name;
            element.content = t.content;

            element.renderer = std::make_unique<Skia::TextRenderer>(canvas);
            element.renderer->setText(t.content);

            element.renderer->textColor.a = 255;
            element.renderer->textColor.r = 0;
            element.renderer->textColor.g = 150;
            element.renderer->textColor.b = 255;

            element.width = element.renderer->getWidth();

            elements.push_back(std::move(element));
        }
    }

    return title;
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
    cursorX = 0;

    for (Element &e : elements)
    {
        if (e.renderer)
        {
            e.renderer->canvas = canvas;

            e.renderer->position.x = e.x;

            e.renderer->init();
            e.renderer->render();
        }
    }

    calculateLayout();
}

void Engine::onResize(int w, int h)
{
    Screen::width = w;
    Screen::height = h;
}

void Engine::calculateLayout()
{
    if (elements.empty())
        return;

    for (Element &e : elements)
    {
        if (e.renderer)
        {
            e.x = cursorX;

            cursorX += e.renderer->getWidth() + 5;
        }
    }
}