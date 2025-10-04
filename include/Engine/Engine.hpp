#include <QVBoxLayout>
#include <QSurfaceFormat>

#include "Engine/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Widget/Widget.hpp"

struct EngineTypes {
    inline static int OpenGL = 0;
    inline static int Vulkan = 1;
};

class Engine {
public:
    Engine();

    void init(Widget *siteContentWidget);
    void setRenderEngine(int type);
    void addRenderLayout(QVBoxLayout *siteContentLayout);

    std::string parse(const std::string &content);
private:
    int type = EngineTypes::OpenGL;
    OpenGLRenderer *openglWindow;
};