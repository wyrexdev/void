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

        canvas->drawString(
            text.c_str(),
            80,
            140,
            font,
            textPaint);
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
} // namespace Skia
