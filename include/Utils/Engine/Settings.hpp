class Settings {
public:
    enum class RenderEngine {
        Skia = 0
    };

    enum class WebEngine {
        Google = 0,
        Yandex = 1,
        Bing = 2,
        Void = 3
    };

    static RenderEngine renderEngine;
    static WebEngine webEngine;
};

Settings::RenderEngine Settings::renderEngine = Settings::RenderEngine::Skia;
Settings::WebEngine Settings::webEngine = Settings::WebEngine::Google;