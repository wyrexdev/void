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
    }

    void EditTextRenderer::onRender()
    {
        text->setX(getX() + 10);
        text->setY(getY() + 10);

        text->init();

        SkPaint pointerPaint;
        pointerPaint.setAntiAlias(true);
        pointerPaint.setColor(SkColorSetRGB(200, 200, 200));

        SkPaint rectPaint;
        rectPaint.setAntiAlias(true);
        rectPaint.setStyle(SkPaint::kStroke_Style);
        rectPaint.setStrokeWidth(2.0f);
        rectPaint.setColor(SkColorSetRGB(200, 200, 200));

        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX(), getY(), text->getWidth() + 30, (text->getHeight() * 2) + 20),
            0, 0,
            rectPaint);
        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX() + text->getWidth() + 25, getY() + 10, 2, (text->getHeight() * 2)),
            0, 0,
            pointerPaint);
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
