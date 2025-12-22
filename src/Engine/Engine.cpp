#include "Engine/Engine.hpp"

Engine::Engine()
{
    screen = QGuiApplication::primaryScreen();
    hz = screen->refreshRate();

    intervalMs = qRound(1000.0 / hz);

    timer = new QTimer();
}

std::string Engine::parse(std::string &content)
{
    Tokenizer *t = new Tokenizer();

    std::string title = "Unknown - Void";

    // HTML Character Entities
    content = String::replaceAll(content, "&nbsp;", " ");  // non-breaking space
    content = String::replaceAll(content, "&lt;", "<");    // less than
    content = String::replaceAll(content, "&gt;", ">");    // greater than
    content = String::replaceAll(content, "&quot;", "\""); // double quotation mark
    content = String::replaceAll(content, "&apos;", "'");  // single quotation mark
    content = String::replaceAll(content, "&cent;", "¢");  // cent
    content = String::replaceAll(content, "&pound;", "£"); // pound
    content = String::replaceAll(content, "&yen;", "¥");   // yen
    content = String::replaceAll(content, "&euro;", "€");  // EURO SIGN
    content = String::replaceAll(content, "&copy;", "©");  // COPYRIGHT
    content = String::replaceAll(content, "&reg;", "®");   // REGISTERED TRADEMARK
    content = String::replaceAll(content, "&trade;", "™"); // trademark

    content = String::replaceAll(content, "&amp;", "&"); // ampersand

    // Combining Diacritical Marks
    content = String::replaceAll(content, "üa&#768;", "à"); // a
    content = String::replaceAll(content, "a&#769;", "á");  // a
    content = String::replaceAll(content, "a&#770;", "â");  // a
    content = String::replaceAll(content, "a&#771;", "ã");  // a
    content = String::replaceAll(content, "O&#768;", "Ò");  // O
    content = String::replaceAll(content, "O&#769;", "Ó");  // O
    content = String::replaceAll(content, "O&#770;", "Ô");  // O
    content = String::replaceAll(content, "O&#771;", "Õ");  // O

    // HTML Symbol Entities
    content = String::replaceAll(content, "&#8592;", "←"); // LEFT ARROW
    content = String::replaceAll(content, "&#8593;", "↑"); // UP ARROW
    content = String::replaceAll(content, "&#8594;", "→"); // RIGHT ARROW
    content = String::replaceAll(content, "&#8595;", "↓"); // DOWN ARROW

    content = String::replaceAll(content, "&#9824;", "♠"); // SPADE
    content = String::replaceAll(content, "&#9827;", "♣"); // CLUB
    content = String::replaceAll(content, "&#9829;", "♥"); // HEART
    content = String::replaceAll(content, "&#9830;", "♦"); // DIAMOND

    this->content = content;

    // HTML Mathematical Entities

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

            element.renderer = std::make_unique<Skia::TextRenderer>(canvas, this);
            element.renderer->setText(t.content);

            element.renderer->setTextColor(255, 255, 255, 255);
            if (t.name == "a")
                element.renderer->setTextColor(255, 0, 150, 255);

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
    QObject::connect(timer, &QTimer::timeout, []()
                     {
    QPoint p = QCursor::pos();
    qDebug() << p; });

    timer->start(intervalMs);
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