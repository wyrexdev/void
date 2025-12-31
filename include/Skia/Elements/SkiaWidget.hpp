#pragma once

#include "Headers/Global.hpp"

class SkiaRenderer;

namespace Skia
{
    class SkiaWidget
    {
    public:
        SkiaWidget();

        void init();
        void render();
        void refresh();

        virtual void onInit() = 0;
        virtual void onRender() = 0;

        Math::Vec3 position;
        Math::Vec3 rotation;
        Math::Vec3 scale;

        SkiaRenderer *parentWidget;
        SkCanvas *canvas;

        bool isHover();

        float getWidth();
        float getHeight();

        void setWidth(float w);
        void setHeight(float h);
    private:
        float width;
        float height;
    };
} // namespace Skia