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

        void initFontFile();

        void setText(std::string text);
        void setFont(std::string path);
        void setSize(float size);

        std::vector<std::string> wrapText(float maxWidth);

    private:
        std::unique_ptr<SkFontScanner> scanner;
        sk_sp<SkFontMgr> fontMgr;
        sk_sp<SkTypeface> typeface;
        SkFont font;
        SkPaint textPaint;

        std::string text;
        std::string fontFile;

        float size;
    };
} // namespace Skia