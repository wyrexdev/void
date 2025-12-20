#include <QVBoxLayout>
#include <QSurfaceFormat>

#include "QT/Widget/Widget.hpp"

class Engine {
public:
    Engine();

    void init(Widget *siteContentWidget);
    void setRenderEngine(int type);
    void addRenderLayout(QVBoxLayout *siteContentLayout);

    std::string parse(const std::string &content);
private:
};