class Settings {
public:
    enum class RenderEngine {
        OpenGL = 0,
        Vulkan = 1
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

Settings::RenderEngine Settings::renderEngine = Settings::RenderEngine::OpenGL;
Settings::WebEngine Settings::webEngine = Settings::WebEngine::Google;