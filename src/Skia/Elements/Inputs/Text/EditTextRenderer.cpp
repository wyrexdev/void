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
    }

    void EditTextRenderer::onRender()
    {
        text->setX(getX());
        text->setY(getY());

        text->init();
        text->render();
    }

    void EditTextRenderer::setText(std::string t) {
        text->setText(t);
    }

    std::string EditTextRenderer::getText() {
        return text->getText();
    }
} // namespace Skia
