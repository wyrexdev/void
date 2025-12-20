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

    private:
        std::unique_ptr<SkFontScanner> scanner;
        sk_sp<SkFontMgr> fontMgr;
        sk_sp<SkTypeface> typeface;
        SkFont font;
        SkPaint textPaint;

        std::string text;
        
    };
} // namespace Skia