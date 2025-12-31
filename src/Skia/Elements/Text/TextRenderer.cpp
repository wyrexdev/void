#include "Skia/Elements/Text/TextRenderer.hpp"

namespace Skia
{
    TextRenderer::TextRenderer(SkCanvas *c, SkiaRenderer *pw)
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

        // float y = position.y + 20;
        // for (const auto &line : wrapText(Screen::width))
        // {
        //     canvas->drawString(line.c_str(), position.x, y, font, textPaint);
        //     y += font.getSize() * 1.2f;
        // }

        canvas->drawString(text.c_str(), position.x, position.y + 20, font, textPaint);

        SkScalar width = font.measureText(
            text.c_str(),
            text.size(),
            SkTextEncoding::kUTF8,
            &bounds);

        setWidth(bounds.width());
        setHeight(bounds.height());
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
} // namespace Skia
