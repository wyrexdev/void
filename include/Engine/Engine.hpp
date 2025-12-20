#include <QVBoxLayout>
#include <QSurfaceFormat>

#include "QT/Widget/Widget.hpp"

class Engine
{
public:
    Engine();

    std::string parse(const std::string &content);

private:
    std::string content;
};