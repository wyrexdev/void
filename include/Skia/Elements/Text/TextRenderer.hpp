#pragma once

#include "QT/Widget/Widget.hpp"

#include "Skia/Elements/SkiaWidget.hpp"

#include "Skia/Math/Color/Color.hpp"

class SkiaRenderer;

namespace Skia
{
    class TextRenderer : public SkiaWidget
    {
    public:
        TextRenderer(SkCanvas *c, SkiaRenderer *parentWidget);

        void onInit() override;
        void onRender() override;

        void initFontFile();

        void setText(std::string text);
        void setFont(std::string path);
        void setSize(float size);

        float getWidth();
        float getHeight();

        Math::Color getTextColor();
        void setTextColor(float a, float r, float g, float b);

        float getWeight();
        void setWeight(float w);

        std::vector<std::string> wrapText(float maxWidth);
    private:
        std::unique_ptr<SkFontScanner> scanner;
        sk_sp<SkFontMgr> fontMgr;
        sk_sp<SkTypeface> typeface;
        SkRect bounds;

        SkFont font;
        SkPaint textPaint;

        std::string text;
        std::string fontFile;

        Math::Color textColor;

        float weight = 0.0f;
        float size;
    };
} // namespace Skia