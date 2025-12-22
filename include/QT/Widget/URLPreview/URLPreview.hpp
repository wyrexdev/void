#pragma once

#include "Headers/Global.hpp"

#include "QT/Widget/Widget.hpp"
#include "QT/Widget/Image/Image.hpp"
#include "QT/Widget/Svg/SvgWidget.hpp"

#include "Utils/QT/Font.hpp"

class URLPreview : public Widget
{
public:
    URLPreview(QWidget *qw) : qwp(qw), Widget(qw)
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

        setAttribute(Qt::WA_AlwaysStackOnTop);
        setAttribute(Qt::WA_TransparentForMouseEvents);

        qwp->installEventFilter(this);

        setupUI();
    }

    QLabel *url;
protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (obj == qwp && event->type() == QEvent::Resize)
        {
            adjustSize();

            int x = 10;
            int y = qwp->height() - height() - 10;

            move(x, y);
            raise();
        }
        return false;
    }

private:
    void setupUI()
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        static QString fontFamily = Font::getNunito();

        url = new QLabel("url preview");
        url->setContentsMargins(4, 4, 0, 4);
        url->setFont(QFont(fontFamily, 8));
        url->setStyleSheet("color:" + QString::fromStdString(Theme::style.textHover) + "; font-weight: 600;");

        mainLayout->addWidget(url);
    }

private:
    QWidget *qwp;
};