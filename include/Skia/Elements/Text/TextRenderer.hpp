#pragma once

#include "Skia/Elements/SkiaWidget.hpp"

namespace Skia
{
    class TextRenderer : public SkiaWidget
    {
    public:
        void onInit() override;
        void onRender() override;
    };
} // namespace Skia