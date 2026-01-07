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
        text->setTextColor(255, 255, 0, 0);
        text->setWeight(200);
    }

    void EditTextRenderer::onRender()
    {
        text->setX(getX());
        text->setY(getY());

        text->init();

        SkPaint rectPaint;
        rectPaint.setAntiAlias(true);
        rectPaint.setColor(SkColorSetRGB(17, 255, 17));

        canvas->drawRoundRect(
            SkRect::MakeXYWH(getX(), getY(), text->getWidth(), text->getHeight() * 2),
            20, 20,
            rectPaint);
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
