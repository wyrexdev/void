#pragma once

#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Skia/Elements/SkiaWidget.hpp"
#include "Skia/Math/Color/Color.hpp"

#include "Headers/SkiaWidgets.hpp"

class SkiaRenderer;

namespace Skia
{
    class EditTextRenderer : public SkiaWidget
    {
        using clock = std::chrono::steady_clock;

    public:
        EditTextRenderer(SkCanvas *c, SkiaRenderer *parentWidget);

        void onInit() override;
        void onRender() override;

        void setText(std::string t);
        std::string getText();

    private:
        TextRenderer *text;

        SkPaint pointerPaint;
        SkPaint borderPaint;

        std::chrono::_V2::steady_clock::time_point last;

        bool isIndic = false;
        float alpha = 255;
    };
}