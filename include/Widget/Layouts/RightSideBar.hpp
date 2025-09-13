#include <QWidget>
#include <QVBoxLayout>
#include <vector>

#include "Widget/Widget.hpp"
#include "Widget/Image/ImageUrl.hpp"
#include "Widget/Svg/SvgWidget.hpp"

class RightSideBar : public Widget
{
public:
    struct RSItem
    {
        std::string icon;
        std::string url;
    };

    RightSideBar() : Widget()
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
        setupUI();
    }

    void addItem(RSItem item)
    {
        RSItems.push_back(item);
        refreshUI();
    }

    void clearItems()
    {
        RSItems.clear();
        refreshUI();
    }

private:
    void setupUI()
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        middleWidget = new QWidget();
        middleLayout = new QVBoxLayout(middleWidget);
        middleLayout->setContentsMargins(0, 0, 0, 0);
        middleLayout->setSpacing(10);
        middleLayout->setAlignment(Qt::AlignCenter);

        Widget *categoryWidget = new Widget();
        categoryWidget->setStyleSheet(
            "background-color: " + QString::fromStdString(Theme::style.surface) + ";" +
            "border-radius: 18px;");
        categoryWidget->setFixedSize(45, 45);

        categoryWidget->setAttribute(Qt::WA_StyledBackground, true);

        SvgWidget *category = new SvgWidget(":/icons/category.svg", QString::fromStdString(Theme::style.text), 22, 22);

        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
        shadowEffect->setBlurRadius(10);
        shadowEffect->setColor(QColor(0, 0, 0, 40));
        shadowEffect->setOffset(0, 2);

        categoryWidget->setGraphicsEffect(shadowEffect);
        categoryWidget->setHoverCursor(Qt::PointingHandCursor);

        categoryWidget->setOnHoverEnter([categoryWidget] {
            QGraphicsDropShadowEffect *hoverShadowEffect = new QGraphicsDropShadowEffect();
            hoverShadowEffect->setBlurRadius(30);
            hoverShadowEffect->setColor(QColor(0, 0, 0, 60));
            hoverShadowEffect->setOffset(0, 3);

            categoryWidget->setGraphicsEffect(hoverShadowEffect);
        });


        categoryWidget->setOnHoverLeave([categoryWidget] {
            QGraphicsDropShadowEffect *leaveShadowEffect = new QGraphicsDropShadowEffect();
            leaveShadowEffect->setBlurRadius(10);
            leaveShadowEffect->setColor(QColor(0, 0, 0, 40));
            leaveShadowEffect->setOffset(0, 2);

            categoryWidget->setGraphicsEffect(leaveShadowEffect);
        });

        QVBoxLayout *categoryLayout = new QVBoxLayout(categoryWidget);
        categoryLayout->setAlignment(Qt::AlignCenter);
        categoryLayout->addWidget(category);

        mainLayout->addWidget(middleWidget, 1);
        mainLayout->addWidget(categoryWidget);
        mainLayout->addSpacerItem(new QSpacerItem(0, 10));
        mainLayout->setAlignment(categoryWidget, Qt::AlignHCenter);
        mainLayout->setAlignment(Qt::AlignVCenter);

        refreshUI();
    }

    void refreshUI()
    {
        QLayoutItem *child;
        while ((child = middleLayout->takeAt(0)) != nullptr)
        {
            delete child->widget();
            delete child;
        }

        for (const RSItem &item : RSItems)
        {
            QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
            shadowEffect->setBlurRadius(10);
            shadowEffect->setColor(QColor(0, 0, 0, 40));
            shadowEffect->setOffset(0, 2);

            Widget *widget = new Widget();
            widget->setStyleSheet("background-color: " + QString::fromStdString(Theme::style.surface) + "; border-radius: 18px;");
            widget->setAttribute(Qt::WA_StyledBackground, true);

            ImageUrl *app = new ImageUrl(QString::fromStdString(item.icon), 45, 45, 18, widget);

            QHBoxLayout *layout = new QHBoxLayout(widget);
            layout->setContentsMargins(2, 2, 2, 2);
            layout->addWidget(app);

            widget->setGraphicsEffect(shadowEffect);
            widget->setHoverCursor(Qt::PointingHandCursor);
            middleLayout->addWidget(widget);
        }
    }

private:
    std::vector<RSItem> RSItems;
    QWidget *middleWidget = nullptr;
    QVBoxLayout *middleLayout = nullptr;
};