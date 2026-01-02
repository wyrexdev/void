#include "Skia/Elements/SkiaWidget.hpp"

namespace Skia
{
    SkiaWidget::SkiaWidget()
    {
    }

    void SkiaWidget::init()
    {
        if(!isInitialized) {
            onInit();
            isInitialized = true;
        }
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
        scale.x = w;
    }

    void SkiaWidget::setHeight(float h) {
        scale.y = h;
    }

    void SkiaWidget::setX(float pX) {
        position.x = pX;
    }

    void SkiaWidget::setY(float pY) {
        position.y = pY;
    }


    float SkiaWidget::getWidth() {
        return scale.x;
    }

    float SkiaWidget::getHeight() {
        return scale.y;
    }

    float SkiaWidget::getX() {
        return position.x;
    }

    float SkiaWidget::getY() {
        return position.y;
    }
} // namespace Skia
