#include "Skia/Elements/Inputs/Text/EditTextRenderer.hpp"

namespace Skia
{
    EditTextRenderer::EditTextRenderer(SkCanvas *c, SkiaRenderer *p)
    {
        canvas = c;
        parentWidget = p;
    }

    void EditTextRenderer::onInit()
    {
        text = new TextRenderer(canvas, parentWidget);
        text->setTextColor(255, 255, 255, 255);
        text->setWeight(200);

        last = clock::now();

        pointerPaint.setAntiAlias(true);
        pointerPaint.setColor(SkColorSetRGB(120, 120, 120));

        borderPaint.setAntiAlias(true);
        borderPaint.setStyle(SkPaint::kStroke_Style);
        borderPaint.setStrokeWidth(2.0f);
        borderPaint.setColor(SkColorSetRGB(200, 200, 200));
    }

    void EditTextRenderer::onRender()
    {
        text->setX(getX() + 5);
        text->setY(getY() + 5);

        text->init();

        auto now = clock::now();

        if (now - last >= std::chrono::seconds(1))
        {
            last = now;
            isIndic = !isIndic;

            if (isIndic)
            {
                pointerPaint.setColor(SkColorSetRGB(120, 120, 120));
            }
            else
            {
                pointerPaint.setColor(SkColorSetARGB(0, 120, 120, 120));
            }
        }

        // Border
        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX(), getY(), 200, (text->getHeight() * 2) + 10),
            5, 5,
            borderPaint);

        // Indicator
        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX() + text->getWidth() + 15, getY() + 5, 2, (text->getHeight() * 2)),
            5, 5,
            pointerPaint);

        // Test Rendering
        text->render();
    }

    void EditTextRenderer::setText(std::string t)
    {
        text->setText(t);
    }

    std::string EditTextRenderer::getText()
    {
        return text->getText();
    }
} // namespace Skia
