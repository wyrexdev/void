#pragma once

#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Skia/Elements/SkiaWidget.hpp"
#include "Skia/Math/Color/Color.hpp"

#include "Headers/SkiaWidgets.hpp"

#include "Engine/Signals/Skia.hpp"
#include "Engine/Signals/System/Keybaord.hpp"

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

        std::atomic<bool> running{true};
        std::atomic<bool> isIndic{true};
        std::atomic<int> alpha{255};
        std::thread blinkThread;
    };
}