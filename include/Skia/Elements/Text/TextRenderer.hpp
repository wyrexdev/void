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
        std::string getText();

        void setFont(std::string path);
        SkFont getFont();

        void setSize(float size);

        Math::Color getTextColor();
        void setTextColor(float a, float r, float g, float b);

        float getWeight();
        void setWeight(float w);

        void selectAll();
        void selectRange(int begin, int end);

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

        float baselineY = 0;

        bool isReUpdateRequired = true;

        sk_sp<SkTextBlob> blob;
    };
} // namespace Skia