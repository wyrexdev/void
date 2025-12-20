#pragma once

#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"

#include "Utils/History.hpp"
#include "Utils/Theme.hpp"

#include "QT/Widget/Svg/SvgWidget.hpp"
#include "QT/Widget/Nav/Tab.hpp"
#include "QT/Widget/Image/Image.hpp"

class Nav : public Widget
{
public:
    SvgWidget *plusIcon;

    struct NItem
    {
        std::string uuid;
        std::string logo;
        std::string name;
        std::string url;
    };

    std::vector<NItem> NItems;

    Nav(QWidget *parent = nullptr) : Widget(parent)
    {
        setupUI();
        refreshUI();

        connect(History::instance(), &History::historyChanged, this, [=](const std::string &uuid, bool added)
                {
            if (!added) {
                removeItem(uuid);
            } });

        connect(History::instance(), &History::currentTabChanged, this, [=](const std::string &uuid)
                {
    static QHash<Tab*, QVariantAnimation*> activeAnimations;
    
    for (int i = 0; i < tabsLayout->count(); ++i) {
        QLayoutItem *item = tabsLayout->itemAt(i);
        if (item && item->widget()) {
            Tab *tab = qobject_cast<Tab *>(item->widget());
            if (!tab) continue;
            
            if (activeAnimations.contains(tab)) {
                QVariantAnimation* oldAnimation = activeAnimations.value(tab);
                if (oldAnimation && oldAnimation->state() == QVariantAnimation::Running) {
                    oldAnimation->stop();
                }
                activeAnimations.remove(tab);
                delete oldAnimation;
            }
            
            QVariantAnimation *animation = new QVariantAnimation(this);
            animation->setDuration(300);
            animation->setEasingCurve(QEasingCurve::InOutQuad);
            
            QColor currentColor = tab->contentWidget->palette().color(QPalette::Window);
            
            QString targetColor;
            if (tab->getUuid() == uuid) {
                targetColor = QString::fromStdString(Theme::style.primary);
            } else {
                targetColor = QString::fromStdString(Theme::style.surface);
            }
            
            animation->setStartValue(currentColor);
            animation->setEndValue(QColor(targetColor));
            
            connect(animation, &QVariantAnimation::valueChanged, this, [tab](const QVariant &value) {
                QColor color = value.value<QColor>();
                QString radius = (color == QColor(QString::fromStdString(Theme::style.primary))) ? "12px" : "10px";
                tab->contentWidget->setStyleSheet(QString("background-color: %1; border-radius: %2;")
                    .arg(color.name())
                    .arg(radius));
            });
            
            connect(animation, &QVariantAnimation::finished, this, [tab, animation, targetColor]() {
                QString radius = (targetColor == QString::fromStdString(Theme::style.primary)) ? "12px" : "10px";
                tab->contentWidget->setStyleSheet(QString("background-color: %1; border-radius: %2;")
                    .arg(targetColor)
                    .arg(radius));
                
                activeAnimations.remove(tab);
                animation->deleteLater();
            });
            
            activeAnimations.insert(tab, animation);
            animation->start();
        }
    } });
    }

    void addItem(NItem item)
    {
        NItems.push_back(item);

        Tab *tab = new Tab(item.uuid, item.logo, item.name, item.url);
        tabsLayout->addWidget(tab);

        History::add(item.uuid, item.url);
    }

    void removeItem(const std::string &uuid)
    {
        for (int i = 0; i < tabsLayout->count(); ++i)
        {
            QLayoutItem *item = tabsLayout->itemAt(i);
            if (item && item->widget())
            {
                Tab *tab = qobject_cast<Tab *>(item->widget());
                if (tab && tab->getUuid() == uuid)
                {
                    tabsLayout->removeWidget(tab);
                    tab->previewWidget->deleteLater();
                    tab->hidePreview();
                    tab->deleteLater();
                    break;
                }
            }
        }

        NItems.erase(std::remove_if(NItems.begin(), NItems.end(), [&](const NItem &item)
                                    { return item.uuid == uuid; }),
                     NItems.end());
    }

    void updateItem(const std::string &uuid, NItem &it)
    {
        for (int i = 0; i < tabsLayout->count(); ++i)
        {
            QLayoutItem *item = tabsLayout->itemAt(i);
            if (item && item->widget())
            {
                Tab *tab = qobject_cast<Tab *>(item->widget());
                if (tab && tab->getUuid() == uuid)
                {
                    tab->logo->setImage(QString::fromStdString(it.logo));
                    tab->tDomain->setText(QString::fromStdString(it.url));
                    tab->label->setText(QString::fromStdString(it.name));
                    break;
                }
            }
        }
    }

    QHBoxLayout *layout;
    QHBoxLayout *tabsLayout;

private:
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

        plusIcon = new SvgWidget(":/icons/plus.svg", QString::fromStdString(Theme::style.textHover), 15, 15);
        layout->addWidget(plusIcon, 0, Qt::AlignVCenter);

        layout->addStretch(1);

        SvgWidget *walletIcon = new SvgWidget(":/icons/wallet.svg", QString::fromStdString(Theme::style.textHover), 21, 21);
        layout->addWidget(walletIcon, 0, Qt::AlignVCenter);

        layout->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

        Image *pp = new Image("https://i.pinimg.com/originals/7b/4c/6d/7b4c6d9638dd629705791d4f9cbe597b.jpg");
        pp->setFixedSize(40, 40);
        pp->setBorderRadius(15);
        pp->setHoverCursor(Qt::PointingHandCursor);
        layout->addWidget(pp);

        layout->setAlignment(Qt::AlignVCenter);
        layout->addStretch();
    }

    void refreshUI()
    {
        History::history.clear();

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
            Tab *tab = new Tab(item.uuid, item.logo, item.name, item.url);
            tabsLayout->addWidget(tab);

            History::add(item.uuid, item.url);
        }
    }
};
