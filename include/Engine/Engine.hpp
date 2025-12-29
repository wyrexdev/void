#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Skia/QT/SkiaRenderer.hpp"

#include "Engine/Parser/Html/Tokenizer.hpp"
#include "Engine/Signals/URLPreviewSignal.hpp"

#include "Engine/Request/Fetcher.hpp"

#include "Engine/Signals/NavSignal.hpp"

#include "Utils/Ram/Ram.hpp"

class Engine : public SkiaRenderer
{
public:
    Engine();

    std::string parse(std::string &content);

    SkiaRenderer *getSkiaView();

    void onInit() override;
    void onRender() override;
    void onResize(int w, int h) override;

    void onMouseDown(float x, float y) override;
    void onMouseUp(float x, float y) override;

    void calculateLayout();

    void pollInput();
    void updateState();

    void redirect(std::string url);

    std::string decodeEntities(const std::string& in);
private:
    QScreen *screen;
    qreal hz;

    int intervalMs;
    bool needsRecalculateLayout = false;

    QTimer *timer;

    std::string content;

    SkiaRenderer *skiaView;

    std::vector<Token> tokens;

    struct Element
    {
        std::unique_ptr<Skia::TextRenderer> renderer;
        std::string name;
        std::string content;
        std::unordered_map<std::string, std::string> attributes;
        float width = 0;
        float height = 0;
        float x = 0;
        float y = 0;
        bool isBlock = false;
    };

    float cursorX = 0, cursorY = 0;

    std::vector<Element> elements;

    QPoint mousePos;

    std::unordered_map<std::string, std::string> htmlEntities = {
        {"&nbsp;", " "},
        {"&lt;", "<"},
        {"&gt;", ">"},
        {"&quot;", "\""},
        {"&apos;", "'"},
        {"&amp;", "&"},
        {"&copy;", "©"},
        {"&reg;", "®"},
        {"&trade;", "™"},
    };
};