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
        if (t.name != "script" && t.name != "style" && t.name != "title")
        {
            Element element;

            element.name = t.name;
            element.content = t.content;

            if (t.name == "div" || t.name == "p" || t.name == "h1" || t.name == "h2" || 
                t.name == "h3" || t.name == "h4" || t.name == "h5" || t.name == "h6" ||
                t.name == "ul" || t.name == "ol" || t.name == "li" || t.name == "section" ||
                t.name == "article" || t.name == "header" || t.name == "footer" ||
                t.name == "nav" || t.name == "main" || t.name == "form")
            {
                element.isBlock = true;
            }
            else if (t.name == "a" || t.name == "span" || t.name == "b" || t.name == "i" ||
                     t.name == "u" || t.name == "strong" || t.name == "em" || t.name == "code")
            {
                element.isBlock = false;
            }
            else
            {
                element.isBlock = false;
            }

            element.renderer = std::make_unique<Skia::TextRenderer>(canvas);
            element.renderer->setText(t.content);

            element.renderer->setTextColor(255, 255, 255, 255);
            if(t.name == "a") element.renderer->setTextColor(255, 0, 150, 255);

            element.renderer->setWeight(200);

            element.width = element.renderer->getWidth();
            element.height = element.renderer->getHeight();

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
    cursorY = 0;
    int maxLineHeight = 0;

    for (Element &e : elements)
    {
        if (e.renderer)
        {
            e.renderer->canvas = canvas;

            e.renderer->position.x = e.x;
            e.renderer->position.y = e.y + cursorY;

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

    cursorX = 0;
    cursorY = 0;
    int maxLineHeight = 0;

    for (Element &e : elements)
    {
        if (e.renderer)
        {
            if (e.isBlock)
            {
                if (cursorX > 0 || cursorY > 0)
                {
                    cursorY += maxLineHeight + 10;
                }
                
                cursorX = 0;
                maxLineHeight = 0;
                
                e.x = cursorX;
                e.y = cursorY;
                
                cursorX = 0;
                cursorY += e.renderer->getHeight() + 10;
            }
            else
            {
                if (cursorX + e.renderer->getWidth() > Screen::width - 20)
                {
                    cursorY += maxLineHeight + 5;
                    cursorX = 0;
                    maxLineHeight = 0;
                }
                
                e.x = cursorX;
                e.y = cursorY;
                
                cursorX += e.renderer->getWidth() + 5;
                
                if (e.renderer->getHeight() > maxLineHeight)
                {
                    maxLineHeight = e.renderer->getHeight();
                }
            }
        }
    }
}