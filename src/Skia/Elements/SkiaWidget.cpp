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

    bool SkiaWidget::isHover()
    {

    }

    void SkiaWidget::setWidth(float w) {
        width = w;
    }

    void SkiaWidget::setHeight(float h) {
        height = h;
    }

    float SkiaWidget::getWidth() {
        return width;
    }

    float SkiaWidget::getHeight() {
        return height;
    }
} // namespace Skia
