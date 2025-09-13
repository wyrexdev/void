#include <QPainter>
#include <QPainterPath>

#include "Utils/Theme.hpp"
#include "Widget/Widget.hpp"

class Preview : public Widget {
public:
    using Widget::Widget;

protected: 
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath path;
        path.addRoundedRect(rect(), 13, 13);

        painter.fillPath(path, QColor(QString::fromStdString(Theme::style.surface)));
    }
};