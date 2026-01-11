#pragma once

#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Skia/Elements/SkiaWidget.hpp"
#include "Skia/Math/Color/Color.hpp"

#include "Headers/SkiaWidgets.hpp"

#include "Engine/Signals/Skia.hpp"
#include "Engine/Signals/System/Keybaord.hpp"
#include "Engine/Signals/System/Mouse.hpp"

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

        void backChar();
        void nextChar();

        void setText(std::string t);
        std::string getText();

        void setHint(std::string h);
        std::string getHint();

        float caretX();
        int nextUtf8Index(const std::string &s, int i);
        int prevUtf8Index(const std::string &s, int i);
    private:
        TextRenderer *text;
        TextRenderer *hint;

        SkPaint pointerPaint;
        SkPaint borderPaint;

        std::atomic<bool> running{true};
        std::atomic<bool> isIndic{true};
        std::atomic<int> alpha{255};
        std::thread blinkThread;

        float indicVal = 0;

        int currentIndex = 0;

        int beginIndex = -1;
        int endIndex = -1;

        bool isSelected = false;
    };
}