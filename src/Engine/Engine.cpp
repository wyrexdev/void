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
    elements.clear();

    std::unique_ptr<HTML::Tokenizer> t = std::make_unique<HTML::Tokenizer>();

    std::string title = "Unknown - Void";
    content = decodeEntities(content);
    this->content = content;
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
            element.attributes = t.attributes;

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

    size_t heap = malloc_usable_size((void *)content.data());
    Signals::Nav::updateCurrentTabHeap(Ram::format_bytes(heap));

    return title;
}

SkiaRenderer *Engine::getSkiaView()
{
    return this;
}

void Engine::pollInput()
{
    mousePos = mapFromGlobal(QCursor::pos());
}

void Engine::onMouseDown(float x, float y)
{
}

void Engine::onMouseUp(float x, float y)
{
    for (Element &e : elements)
    {
        if (!e.renderer)
            continue;

        if (e.name != "a")
            continue;

        if (
            x >= e.x &&
            x <= e.x + e.width &&
            y >= e.y &&
            y <= e.y + e.height)
        {
            redirect(e.attributes["href"]);
        }
    }
}

void Engine::updateState()
{
    bool anyHover = false;
    std::string url = "";

    for (Element &e : elements)
    {
        if (!e.renderer)
            continue;

        float startX = e.x;
        float endX = e.x + e.width;
        float startY = e.y;
        float endY = e.y + e.height;

        if (
            mousePos.x() >= startX &&
            mousePos.x() <= endX &&
            mousePos.y() >= startY &&
            mousePos.y() <= endY)
        {
            url = e.attributes["href"];
            anyHover = true;
            break;
        }
    }

    if (anyHover)
    {
        setCursor(Qt::PointingHandCursor);
        Signals::URLPReview::show(url);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        Signals::URLPReview::close();
    }

    needsRecalculateLayout = true;
}

void Engine::onInit()
{
    QObject::connect(timer, &QTimer::timeout, this, [this]()
                     {
    pollInput();
    updateState(); });

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

    if (needsRecalculateLayout)
    {
        calculateLayout();
        needsRecalculateLayout = false;
    }
}

void Engine::onResize(int w, int h)
{
    Screen::width = w;
    Screen::height = h;

    needsRecalculateLayout = true;
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

                e.width = e.renderer->getWidth();
                e.height = e.renderer->getHeight();

                cursorX += e.renderer->getWidth() + 5;

                if (e.renderer->getHeight() > maxLineHeight)
                {
                    maxLineHeight = e.renderer->getHeight();
                }
            }
        }
    }
}

void Engine::redirect(std::string url)
{
    Fetcher *fetcher = new Fetcher();
    std::string content = fetcher->get(url);

    parse(content);
}

std::string Engine::decodeEntities(const std::string &in)
{
    std::string out;
    out.reserve(in.size());

    for (size_t i = 0; i < in.size(); ++i)
    {
        if (in[i] == '&')
        {
            auto end = in.find(';', i);
            if (end != std::string::npos)
            {
                auto key = in.substr(i, end - i + 1);
                auto it = htmlEntities.find(key);
                if (it != htmlEntities.end())
                {
                    out += it->second;
                    i = end;
                    continue;
                }
            }
        }
        out += in[i];
    }
    return out;
}
