#include "Skia/Elements/Text/TextRenderer.hpp"

namespace Skia
{
    TextRenderer::TextRenderer(SkCanvas *c, SkiaRenderer *pw) : textColor(Math::Color{255, 255, 255, 0})
    {
        parentWidget = pw;
        canvas = c;

        size = 16;
        text = "";
        fontFile = "./fonts/nunito.ttf";
    }

    void TextRenderer::onInit()
    {
        scanner = SkFontScanner_Make_FreeType();
        fontMgr = SkFontMgr_New_FontConfig(nullptr, std::move(scanner));

        initFontFile();

        if (!typeface)
        {
            qWarning() << "Font cannot load!";
            return;
        }

        font = SkFont(typeface, size);
        font.setEdging(SkFont::Edging::kSubpixelAntiAlias);

        textPaint.setAntiAlias(true);
        textPaint.setColor(SkColorSetARGB(textColor.a, textColor.r, textColor.g, textColor.b));

        textPaint.setStyle(SkPaint::kStrokeAndFill_Style);
        textPaint.setStrokeWidth(weight / 500);

        setWidth(bounds.width());
        setHeight(bounds.height());
    }

    void TextRenderer::onRender()
    {
        if (!typeface)
        {
            return;
        }

        blob = SkTextBlob::MakeFromString(
            text.c_str(),
            font);

        if (blob)
        {
            SkScalar width = font.measureText(
                text.c_str(),
                text.size(),
                SkTextEncoding::kUTF8,
                &bounds);

            setWidth(width);
            setHeight(bounds.height());

            SkFontMetrics metrics;
            font.getMetrics(&metrics);

            baselineY = getY() - metrics.fAscent;

            SkScalar x = getX();
            SkScalar y = baselineY;

            SkRect selectedArea = SkRect::MakeLTRB(
                x + bounds.left(),
                y + bounds.top(),
                x + bounds.right(),
                y + bounds.bottom());

            selectedArea.outset(0, 3);

            SkPaint bgPaint;
            bgPaint.setAntiAlias(true);

            if (isSelected)
            {
                bgPaint.setColor(SkColorSetARGB(255, 53, 132, 228));
            } else {
                bgPaint.setColor(SkColorSetARGB(0, 53, 132, 228));
            }

            canvas->drawRect(selectedArea, bgPaint);
            canvas->drawTextBlob(blob, getX(), baselineY, textPaint);
        }
    }

    void TextRenderer::initFontFile()
    {
        typeface = fontMgr->makeFromFile(fontFile.c_str());
    }

    void TextRenderer::setFont(std::string f)
    {
        fontFile = f;

        initFontFile();
        refresh();
    }

    void TextRenderer::setText(std::string t)
    {
        text = t;
        refresh();
    }

    std::string TextRenderer::getText()
    {
        return text;
    }

    void TextRenderer::setSize(float s)
    {
        size = s;
        refresh();
    }

    std::vector<std::string> TextRenderer::wrapText(float maxWidth)
    {
        std::vector<std::string> lines;
        std::string line;
        std::string word;

        for (char c : text)
        {
            if (c == ' ' || c == '\n')
            {
                std::string test = line + word + " ";
                float w = font.measureText(
                    test.c_str(),
                    test.size(),
                    SkTextEncoding::kUTF8);

                if (w > maxWidth && !line.empty())
                {
                    lines.push_back(line);
                    line.clear();
                }

                line += word + " ";
                word.clear();

                if (c == '\n')
                {
                    lines.push_back(line);
                    line.clear();
                }
            }
            else
            {
                word += c;
            }
        }

        if (!word.empty())
        {
            std::string test = line + word;
            float w = font.measureText(
                test.c_str(),
                test.size(),
                SkTextEncoding::kUTF8);

            if (w > maxWidth && !line.empty())
            {
                lines.push_back(line);
                line = word;
            }
            else
            {
                line += word;
            }
        }

        if (!line.empty())
            lines.push_back(line);

        return lines;
    }

    Math::Color TextRenderer::getTextColor()
    {
        return textColor;
    }

    void TextRenderer::setTextColor(float a, float r, float g, float b)
    {
        textColor.a = a;
        textColor.r = r;
        textColor.g = g;
        textColor.b = b;
    }

    float TextRenderer::getWeight()
    {
        return weight;
    }

    void TextRenderer::setWeight(float w)
    {
        weight = w;
    }

    SkFont TextRenderer::getFont()
    {
        return font;
    }

    void TextRenderer::selectAll()
    {
        beginIndex = 0;
        endIndex = text.size();

        isSelected = true;

        QObject::connect(Signals::System::Mouse::instance(), &Signals::System::Mouse::onClick, [=](float x, float y)
                         { QMetaObject::invokeMethod(
                               Signals::Skia::instance(),
                               "updateSkia",
                               Qt::QueuedConnection); });
    }

    void TextRenderer::selectRange(int begin, int end)
    {
    }

    void TextRenderer::unselect()
    {
        isSelected = false;

        beginIndex = -1;
        endIndex = -1;

        QObject::connect(Signals::System::Mouse::instance(), &Signals::System::Mouse::onClick, [=](float x, float y)
                         { QMetaObject::invokeMethod(
                               Signals::Skia::instance(),
                               "updateSkia",
                               Qt::QueuedConnection); });
    }
} // namespace Skia
