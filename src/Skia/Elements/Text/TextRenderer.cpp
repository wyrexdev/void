#include "Skia/Elements/Text/TextRenderer.hpp"

namespace Skia
{
    TextRenderer::TextRenderer(SkCanvas *c)
    {
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
        textPaint.setColor(SK_ColorWHITE);
    }

    void TextRenderer::onRender()
    {
        if (!typeface)
        {
            qWarning() << "Font cannot load!";
            return;
        }

        float y = 30;
        for (const auto &line : wrapText(400))
        {
            canvas->drawString(line.c_str(), 20, y, font, textPaint);
            y += font.getSize() * 1.2f;
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
} // namespace Skia
