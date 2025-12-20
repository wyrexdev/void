#pragma once

#include "Skia/Elements/SkiaWidget.hpp"

namespace Skia
{
    class TextRenderer : public SkiaWidget
    {
    public:
        TextRenderer(SkCanvas *c);
        
        void onInit() override;
        void onRender() override;
    };
} // namespace Skia