#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Skia/QT/SkiaRenderer.hpp"

class Engine : public SkiaRenderer
{
public:
    Engine();

    std::string parse(const std::string &content);

    SkiaRenderer *getSkiaView();

    void onInit() override;
    void onRender() override;
    void onResize(int w, int h) override;

private:
    std::string content;

    SkiaRenderer *skiaView;

    Skia::TextRenderer *t;
};