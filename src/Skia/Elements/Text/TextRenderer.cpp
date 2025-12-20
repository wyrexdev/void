#include "Skia/Elements/Text/TextRenderer.hpp"

namespace Skia
{
    TextRenderer::TextRenderer(SkCanvas *c)
    {
        canvas = c;
    }

    void TextRenderer::onInit()
    {
        scanner = SkFontScanner_Make_FreeType();
        fontMgr = SkFontMgr_New_FontConfig(nullptr, std::move(scanner));

        typeface = fontMgr->makeFromFile("./fonts/nunito.ttf");

        if (!typeface)
        {
            qWarning() << "Font cannot load!";
            return;
        }

        font = SkFont(typeface, 32);
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
} // namespace Skia
