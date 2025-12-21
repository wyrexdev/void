#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Skia/QT/SkiaRenderer.hpp"

#include "Engine/Parser/Html/Tokenizer/Tokenizer.hpp"

#include "Utils/String/String.hpp"

class Engine : public SkiaRenderer
{
public:
    Engine();

    std::string parse(std::string &content);

    SkiaRenderer *getSkiaView();

    void onInit() override;
    void onRender() override;
    void onResize(int w, int h) override;

    void calculateLayout();

private:
    std::string content;

    SkiaRenderer *skiaView;

    std::vector<Token> tokens;

    struct Element
    {
        std::unique_ptr<Skia::TextRenderer> renderer;
        std::string name;
        std::string content;
        float width = 0;
        float height = 0;
        float x = 0;
        float y = 0;
        bool isBlock = false;
    };

    float cursorX = 0, cursorY = 0;

    std::vector<Element> elements;
};