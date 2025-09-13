#include <QWidget>
#include <QHBoxLayout>
#include <QSvgRenderer>
#include <QPainter>
#include <QFile>
#include <QLabel>
#include <QSpacerItem>
#include <vector>

#include "Utils/Theme.hpp"
#include "Widget/Svg/SvgWidget.hpp"
#include "Widget/Nav/Tab.hpp"
#include "Widget/Image/ImageUrl.hpp"

#include "Widget/Widget.hpp"

class Nav : public Widget
{
public:
    struct NItem
    {
        std::string logo;
        std::string name;
        std::string url;
    };

    Nav(QWidget *parent = nullptr) : Widget(parent)
    {
        setupUI();
        refreshUI();
    }

    void addItem(NItem item)
    {
        NItems.push_back(item);
        refreshUI();
    }

private:
    QHBoxLayout *layout;
    QHBoxLayout *tabsLayout;

    std::vector<NItem> NItems;

    void setupUI()
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setFixedHeight(60);

        layout = new QHBoxLayout(this);

        SvgWidget *leftArrow = new SvgWidget(":/icons/arrowLeft.svg", QString::fromStdString(Theme::style.text), 20, 20);
        layout->addWidget(leftArrow);

        SvgWidget *rightArrow = new SvgWidget(":/icons/arrowRight.svg", QString::fromStdString(Theme::style.text), 20, 20);
        layout->addWidget(rightArrow);

        layout->addSpacerItem(new QSpacerItem(1, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        SvgWidget *refreshIcon = new SvgWidget(":/icons/refresh.svg", QString::fromStdString(Theme::style.text), 13, 13);
        layout->addWidget(refreshIcon);

        layout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        QWidget *line = new QWidget();
        line->setFixedSize(2, 9);
        line->setStyleSheet(
            "background-color: " + QString::fromStdString(Theme::style.text) + ";"
                                                                                   "border-radius: 15px;");
        layout->addWidget(line);

        layout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        tabsLayout = new QHBoxLayout();
        layout->addLayout(tabsLayout);

        layout->addSpacerItem(new QSpacerItem(2, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        SvgWidget *plusIcon = new SvgWidget(":/icons/plus.svg", QString::fromStdString(Theme::style.textHover), 15, 15);
        layout->addWidget(plusIcon, 0, Qt::AlignVCenter);

        layout->addStretch(1);

        SvgWidget *walletIcon = new SvgWidget(":/icons/wallet.svg", QString::fromStdString(Theme::style.textHover), 21, 21);
        layout->addWidget(walletIcon, 0, Qt::AlignVCenter);

        layout->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        ImageUrl *pp = new ImageUrl("https://avatars.mds.yandex.net/i?id=3dbb34e4d6325cd195e508b97c5837e09b1c8c68-17021924-images-thumbs&n=13", 40, 40, 15);
        pp->setHoverCursor(Qt::PointingHandCursor);
        layout->addWidget(pp);

        layout->setAlignment(Qt::AlignVCenter);
        layout->addStretch();
    }

    void refreshUI()
    {
        for (int i = tabsLayout->count() - 1; i >= 0; --i)
        {
            QLayoutItem *item = tabsLayout->itemAt(i);
            if (item && item->widget())
            {
                Tab *tab = qobject_cast<Tab *>(item->widget());
                if (tab)
                {
                    tabsLayout->removeWidget(tab);
                    delete tab;
                }
            }
        }

        for (NItem item : NItems)
        {
            Tab *tab = new Tab(item.logo, item.name, item.url);
            tabsLayout->addWidget(tab);
        }
    }
};
