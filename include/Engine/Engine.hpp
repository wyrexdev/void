#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Skia/QT/SkiaRenderer.hpp"

class Engine
{
public:
    Engine();

    std::string parse(const std::string &content);

    SkiaRenderer *getSkiaView();

private:
    std::string content;

    SkiaRenderer *skiaView;
};