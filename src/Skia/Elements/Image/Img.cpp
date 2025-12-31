#include "Skia/Elements/Image/Img.hpp"

namespace Skia
{
    ImgRenderer::ImgRenderer(SkCanvas *c, SkiaRenderer *pw)
    {
        parentWidget = pw;
        canvas = c;
    }

    void ImgRenderer::onInit()
    {
        bodyData = loadFile("./images/VB.png");

        data = SkData::MakeWithoutCopy(
            bodyData.data(),
            bodyData.size());

        codec = SkCodec::MakeFromData(data);

        if (!codec)
        {
        }

        info = SkImageInfo::Make(
            codec->getInfo().width(),
            codec->getInfo().height(),
            kRGBA_8888_SkColorType,
            kPremul_SkAlphaType);

        bitmap.allocPixels(info);

        codec->getPixels(info, bitmap.getPixels(), bitmap.rowBytes());

        image = SkImages::RasterFromBitmap(bitmap);
    }

    void ImgRenderer::onRender()
    {
        SkRect dst = SkRect::MakeXYWH(getX(), getY(), getWidth(), getHeight());

        canvas->drawImageRect(
            image.get(),
            dst,
            SkSamplingOptions(),
            nullptr);
    }

    std::vector<uint8_t> ImgRenderer::loadFile(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            throw std::runtime_error("file open failed");

        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        file.read(reinterpret_cast<char *>(buffer.data()), size);

        return buffer;
    }
} // namespace name
