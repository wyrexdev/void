#include "Skia/Elements/SkiaWidget.hpp"

namespace Skia
{
    SkiaWidget::SkiaWidget()
    {
    }

    void SkiaWidget::init()
    {
        onInit();
    }

    void SkiaWidget::render()
    {
        onRender();
    }

    void SkiaWidget::refresh()
    {
        render();
    }
} // namespace Skia
