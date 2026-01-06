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
        hint = new TextRenderer(canvas, parentWidget);
        hint->init();
    }

    void EditTextRenderer::onRender()
    {
        hint->render();
    }

    void EditTextRenderer::keyPressEvent(QKeyEvent *event)
    {
        qDebug() << "Basılan tuş:" << event->key();
    }

} // namespace Skia
