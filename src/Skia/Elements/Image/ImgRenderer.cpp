#include "Skia/Elements/Image/ImgRenderer.hpp"

namespace Skia
{
    ImgRenderer::ImgRenderer(SkCanvas *c, SkiaRenderer *pw)
    {
        parentWidget = pw;
        canvas = c;
    }

    void ImgRenderer::onInit()
    {
    }

    void ImgRenderer::onRender()
    {
        if (!image)
            return;

        SkRect dst = SkRect::MakeXYWH(getX(), getY(), getWidth(), getHeight());

        canvas->drawImageRect(
            image,
            dst,
            SkSamplingOptions(),
            nullptr);
    }

    void ImgRenderer::loadImage(std::vector<uint8_t> d)
    {
        if (d.empty())
            return;

        bodyData = std::move(d);

        data = SkData::MakeWithoutCopy(
            bodyData.data(),
            bodyData.size());

        codec = SkCodec::MakeFromData(data);
        if (!codec)
        {
            image.reset();
            return;
        }

        info = SkImageInfo::Make(
            codec->getInfo().width(),
            codec->getInfo().height(),
            kRGBA_8888_SkColorType,
            kPremul_SkAlphaType);

        bitmap.allocPixels(info);

        auto res = codec->getPixels(info, bitmap.getPixels(), bitmap.rowBytes());
        if (res != SkCodec::kSuccess)
        {
            image.reset();
            return;
        }

        image = SkImages::RasterFromBitmap(bitmap);
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

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t totalSize = size * nmemb;
        auto *buffer = static_cast<std::vector<uint8_t> *>(userp);

        uint8_t *data = static_cast<uint8_t *>(contents);
        buffer->insert(buffer->end(), data, data + totalSize);

        return totalSize;
    }

    std::vector<uint8_t> ImgRenderer::loadFromUrl(const std::string &url)
    {
        CURL *curl = curl_easy_init();
        // if (!curl)
        // throw std::runtime_error("curl init failed");

        std::vector<uint8_t> buffer;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            // throw std::runtime_error(curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        return buffer;
    }
} // namespace name
