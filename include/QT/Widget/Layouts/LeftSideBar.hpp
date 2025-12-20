#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QGraphicsEffect>

#include "QT/Widget/Widget.hpp"
#include "QT/Widget/Svg/SvgWidget.hpp"
#include "QT/Widget/Layouts/LeftBar/LSItem.hpp"

class LeftSideBar : public Widget
{
public:
    LeftSideBar() : Widget()
    {
        setStyleSheet(
            "background-color: " + QString::fromStdString(Theme::style.surface) + ";" +
            "border-radius: 15px;");

        setContentsMargins(0, 0, 0, 0);

        setAttribute(Qt::WA_StyledBackground, true);

        QVBoxLayout *layout = new QVBoxLayout(this);

        // --- HOME --- //
        LSItem* home = new LSItem(":/icons/home.svg");
        layout->addWidget(home);
        // --- HOME END --- //

        // --- SETTINGS --- //
        LSItem* settings = new LSItem(":/icons/settings.svg");
        layout->addWidget(settings);
        // --- SETTINGS END --- //

        layout->setSizeConstraint(QLayout::SetFixedSize);
    }
};