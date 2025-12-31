#pragma once

#include <fstream>
#include <vector>

#include "QT/Widget/Widget.hpp"

#include "Skia/Elements/SkiaWidget.hpp"

#include "Skia/Math/Color/Color.hpp"

class SkiaRenderer;

namespace Skia
{
    class ImgRenderer : public SkiaWidget
    {
    public:
        ImgRenderer(SkCanvas *c, SkiaRenderer *parentWidget);

        void onInit() override;
        void onRender() override;

        std::vector<uint8_t> loadFile(const std::string &path);

    private:
        sk_sp<SkImage> image;
        SkImageInfo info;
        SkBitmap bitmap;
        sk_sp<SkData> data;
        std::vector<uint8_t> bodyData;
        std::unique_ptr<SkCodec> codec;

        int width = 5;
        int height = 5;
    };
} // namespace Skia