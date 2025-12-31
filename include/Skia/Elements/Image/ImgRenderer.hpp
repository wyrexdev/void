#pragma once

#include <fstream>
#include <vector>
#include <cstdint>
#include <curl/curl.h>

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

        void loadImage(std::vector<uint8_t> data);

        std::vector<uint8_t> loadFile(const std::string &path);
        std::vector<uint8_t> loadFromUrl(const std::string &url);

    private:
        sk_sp<SkImage> image;
        SkImageInfo info;
        SkBitmap bitmap;
        sk_sp<SkData> data;
        std::vector<uint8_t> bodyData;
        std::unique_ptr<SkCodec> codec;
    };
} // namespace Skia