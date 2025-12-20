#pragma once

#include "Headers/Global.hpp"

namespace Skia
{
    class SkiaWidget
    {
    public:
        SkiaWidget();

        void init();
        void render();

        virtual void onInit() = 0;
        virtual void onRender() = 0;

        Math::Vec3 position;
        Math::Vec3 rotation;
        Math::Vec3 scale;

    private:
    };
} // namespace Skia