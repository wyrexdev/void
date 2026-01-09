#include "Skia/Elements/SkiaWidget.hpp"

namespace Skia
{
    SkiaWidget::SkiaWidget()
    {
    }

    void SkiaWidget::init()
    {
        if (!isInitialized)
        {
            onInit();
            isInitialized = true;
        }
    }

    void SkiaWidget::render()
    {
        if (enabled)
        {
            onRender();
        }
    }

    void SkiaWidget::refresh()
    {
        if (enabled)
        {
            render();
        }
    }

    bool SkiaWidget::isHover()
    {
    }

    void SkiaWidget::setWidth(float w)
    {
        scale.x = w;
    }

    void SkiaWidget::setHeight(float h)
    {
        scale.y = h;
    }

    void SkiaWidget::setX(float pX)
    {
        position.x = pX;
    }

    void SkiaWidget::setY(float pY)
    {
        position.y = pY;
    }

    void SkiaWidget::setMinHeight(float h)
    {
        minHeight = h;
    }

    void SkiaWidget::setMaxHeight(float h)
    {
        maxHeight = h;
    }

    void SkiaWidget::setMinWidth(float w)
    {
        minWidth = w;
    }

    void SkiaWidget::setMaxWidth(float w)
    {
        maxWidth = w;
    }

    float SkiaWidget::getWidth()
    {
        return scale.x;
    }

    float SkiaWidget::getHeight()
    {
        return scale.y;
    }

    float SkiaWidget::getX()
    {
        return position.x;
    }

    float SkiaWidget::getY()
    {
        return position.y;
    }

    float SkiaWidget::getMinHeight()
    {
        return minHeight;
    }

    float SkiaWidget::getMaxHeight()
    {
        return maxHeight;
    }

    float SkiaWidget::getMinWidth()
    {
        return minWidth;
    }

    float SkiaWidget::getMaxWidth()
    {
        return maxWidth;
    }

    bool SkiaWidget::isEnabled()
    {
        return enabled;
    }

    void SkiaWidget::enable()
    {
        enabled = true;
    }

    void SkiaWidget::disable()
    {
        enabled = false;
    }
} // namespace Skia
