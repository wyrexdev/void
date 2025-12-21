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

    for(Token token : tokens) {
        if(token.name == "title") {
            title = token.content;
            continue;
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
    for(Token t : tokens) {
        std::cout << t.name << std::endl;
        if(t.name == "a") {
            
        }
    }

    Skia::TextRenderer *text = new Skia::TextRenderer(canvas);
            text->setText(content);

            text->init();
            text->render();
}

void Engine::onResize(int w, int h)
{
    Screen::width = w;
    Screen::height = h;
}