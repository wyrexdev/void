#pragma once

#include "QT/Widget/Widget.hpp"

#include "Skia/Elements/SkiaWidget.hpp"
#include "Skia/Math/Color/Color.hpp"

#include "Headers/SkiaWidgets.hpp"

class SkiaRenderer;

namespace Skia
{
    class EditTextRenderer : public SkiaWidget
    {
    public:
        EditTextRenderer(SkCanvas *c, SkiaRenderer *parentWidget);

        void onInit() override;
        void onRender() override;

    private:
        TextRenderer *hint;
    };
}