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

        SkiaRenderer *parentWidget;
        SkCanvas *canvas;

        bool isHover();

        float getWidth();
        float getHeight();

        float getX();
        float getY();

        void setWidth(float w);
        void setHeight(float h);

        void setX(float x);
        void setY(float y);

    private:
        Math::Vec3 position;
        Math::Vec3 rotation;
        Math::Vec3 scale;

        bool isInitialized = false;
    };
} // namespace Skia