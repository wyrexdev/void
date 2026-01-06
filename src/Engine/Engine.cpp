#include "Engine/Engine.hpp"

Engine::Engine()
{
    screen = QGuiApplication::primaryScreen();
    hz = screen->refreshRate();

    intervalMs = qRound(1000.0 / hz);

    timer = new QTimer();
}

Engine::DocumentMetada Engine::parse(std::string &content)
{
    elements.clear();

    std::unique_ptr<HTML::Tokenizer> t = std::make_unique<HTML::Tokenizer>();

    std::string title = "Unknown - Void";
    std::string logoUrl = "https://cdn.vobrow.com/logo.png";

    content = decodeEntities(content);
    this->content = content;
    tokens = t->tokenize(content);

    for (HTML::Tokenizer::Token t : tokens)
    {
        if (t.name == "title")
        {
            title = t.content;
        }

        if (t.name == "link")
        {
            auto relR = t.attributes.find("rel");
            if (relR != t.attributes.end())
            {
                if (relR->second == "icon")
                {
                    auto hrefR = t.attributes.find("href");
                    if (hrefR != t.attributes.end())
                    {
                        logoUrl = hrefR->second;
                    }
                }
            }
        }

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

            if (t.name == "img")
            {
                std::unique_ptr<Skia::ImgRenderer> imgRenderer = std::make_unique<Skia::ImgRenderer>(canvas, this);
                imgRenderer.get()->setHeight(600);
                imgRenderer.get()->setWidth(600);

                auto srcR = t.attributes.find("src");
                if (srcR != t.attributes.end())
                {
                    std::string src = NetworkLoader::urlDecode(srcR->second);
                    std::string finalUrl;

                    if (NetworkLoader::isAbsoluteURL(src))
                    {
                        finalUrl = src;
                    }
                    else if (!src.empty() && src[0] == '/')
                    {
                        finalUrl = NetworkLoader::getOrigin(getURL()) + src;
                    }
                    else
                    {
                        finalUrl = NetworkLoader::getBaseDir(getURL()) + src;
                    }

                    imgRenderer->loadImage(
                        imgRenderer->loadFromUrl(finalUrl));
                }

                auto wR = t.attributes.find("width");
                if (wR != t.attributes.end())
                {
                    imgRenderer.get()->setWidth(std::stof(wR->second));
                }

                auto hR = t.attributes.find("height");
                if (hR != t.attributes.end())
                {
                    imgRenderer.get()->setHeight(std::stof(hR->second));
                }

                // imgRenderer.get()->loadImage(imgRenderer.get()->loadFromUrl("https://cdn.vobrow.com/logo.png"));
                element.renderer = std::move(imgRenderer);
            }
            else if (t.name == "h1" || t.name == "h2" || t.name == "h3" || t.name == "h4" || t.name == "h5" || t.name == "h6" || t.name == "a" || t.name == "p")
            {
                std::unique_ptr<Skia::TextRenderer> textRenderer = std::make_unique<Skia::TextRenderer>(canvas, this);
                textRenderer.get()->setText(t.content);

                textRenderer.get()->setTextColor(255, 255, 255, 255);
                if (t.name == "a")
                    textRenderer.get()->setTextColor(255, 0, 150, 255);

                textRenderer.get()->setWeight(200);

                element.renderer = std::move(textRenderer);
            }

            elements.push_back(std::move(element));
        }
    }

    size_t heap = malloc_usable_size((void *)content.data());
    Signals::Nav::updateCurrentTabHeap(Ram::format_bytes(heap));

    return {
        title,
        logoUrl};
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
            x >= e.renderer->getX() &&
            x <= e.renderer->getX() + e.renderer->getWidth() &&
            y >= e.renderer->getY() &&
            y <= e.renderer->getY() + e.renderer->getHeight())
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

        float startX = e.renderer->getX();
        float endX = e.renderer->getX() + e.renderer->getWidth();
        float startY = e.renderer->getY();
        float endY = e.renderer->getY() + e.renderer->getHeight();

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

    editText = new Skia::EditTextRenderer(canvas, this);
}

void Engine::onRender()
{
    cursorX = 0;
    cursorY = 0;
    int maxLineHeight = 0;

    canvas->save();
    canvas->translate(0, scrollY);

    for (Element &e : elements)
    {
        if (e.renderer)
        {
            e.renderer->canvas = canvas;

            e.renderer->setX(e.renderer->getX());
            e.renderer->setY(e.renderer->getY() + cursorY);

            e.renderer->init();
            e.renderer->render();
        }
    }

    if (needsRecalculateLayout)
    {
        calculateLayout();
        needsRecalculateLayout = false;
    }

    editText->canvas = canvas;

    editText->setX(100);
    editText->setY(200);

    editText->init();
    editText->render();

    canvas->restore();
}

void Engine::onResize(int w, int h)
{
    needsRecalculateLayout = true;
}

void Engine::calculateLayout()
{
    if (elements.empty())
    {
        totalHeight = 0;
        return;
    }

    cursorX = 0;
    cursorY = 0;
    int maxLineHeight = 0;

    for (Element &e : elements)
    {
        if (!e.renderer)
            continue;

        if (e.isBlock)
        {
            if (cursorX > 0)
            {
                cursorY += maxLineHeight + 10;
                cursorX = 0;
                maxLineHeight = 0;
            }

            e.renderer->setX(0);
            e.renderer->setY(cursorY);

            cursorY += e.renderer->getHeight() + 10;
        }
        else
        {
            if (cursorX + e.renderer->getWidth() > Skia::Viewport::width - 20)
            {
                cursorY += maxLineHeight + 5;
                cursorX = 0;
                maxLineHeight = 0;
            }

            e.renderer->setX(cursorX);
            e.renderer->setY(cursorY);

            cursorX += e.renderer->getWidth() + 5;
            maxLineHeight = std::max(maxLineHeight, (int)e.renderer->getHeight());
        }
    }

    totalHeight = cursorY + maxLineHeight;
}

void Engine::redirect(std::string url)
{
    Core::Resource res = NetworkLoader::get(url);
    parse(res.body);
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

void Engine::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key Pressed:" << event->key();

    editText->setText(
        editText->getText() + event->text().toStdString());

    update();
}