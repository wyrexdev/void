#include <QWidget>
#include <QHBoxLayout>
#include <QFontDatabase>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSizePolicy>

#include "Widget/Widget.hpp"
#include "Widget/Nav/Preview/Preview.hpp"
#include "Widget/Image/Image.hpp"
#include "Widget/Svg/SvgWidget.hpp"
#include "Utils/Theme.hpp"

class Tab : public Widget
{
    Q_OBJECT
public:
    ~Tab()
    {
        delete showAnimation;
        delete hideAnimation;
    }

    Tab(std::string logoUrl, std::string name, std::string url) : Widget()
    {
        setupUi(logoUrl, name, url);
        setupPreview(name);
        setupLayout();
        setupAnimations();

        contentWidget->installEventFilter(this);
        installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (obj == contentWidget || obj == this)
        {
            if (event->type() == QEvent::Enter)
            {
                showPreview();
            }
            else if (event->type() == QEvent::Leave)
            {
                hidePreview();
            }
        }
        else if (obj == previewWidget)
        {
            if (event->type() == QEvent::Enter)
            {
                hidePreviewTimer.stop();
            }
            else if (event->type() == QEvent::Leave)
            {
                QTimer::singleShot(200, this, &Tab::hidePreview);
            }
        }

        return Widget::eventFilter(obj, event);
    }

private:
    Preview *previewWidget;
    Widget *contentWidget;
    QTimer hidePreviewTimer;
    QPropertyAnimation *showAnimation;
    QPropertyAnimation *hideAnimation;
    QGraphicsOpacityEffect *opacityEffect;

    void setupUi(const std::string &logoUrl, const std::string &name, const std::string &url)
    {
        setAttribute(Qt::WA_StyledBackground, true);
        setHoverCursor(Qt::PointingHandCursor);

        contentWidget = new Widget(this);
        contentWidget->setFixedSize(155, 35);
        contentWidget->setStyleSheet(
            "background-color: " + QString::fromStdString(Theme::style.surface) + ";" +
            "border-radius: 10px;");
        contentWidget->setAttribute(Qt::WA_StyledBackground, true);

        contentWidget->setHoverCursor(Qt::PointingHandCursor);

        QHBoxLayout *content = new QHBoxLayout(contentWidget);
        content->setContentsMargins(8, 0, 8, 0);

        Image *logo = new Image(QString::fromStdString(logoUrl));
        logo->setFixedSize(20, 20);
        logo->setBorderRadius(8);
        
        content->addWidget(logo);
        logo->setHoverCursor(Qt::PointingHandCursor);

        content->addLayout(createTextLayout(name, url));

        SvgWidget *cancelIcon = new SvgWidget(":/icons/cancel.svg",
                                              QString::fromStdString(Theme::style.textHover), 8, 8, contentWidget);
        content->addWidget(cancelIcon, 0, Qt::AlignVCenter);
        cancelIcon->setHoverColor(QString::fromStdString(Theme::style.text));
    }

    QVBoxLayout *createTextLayout(const std::string &name, const std::string &url)
    {
        static QString fontFamily = loadFontFamily();

        QVBoxLayout *contentLayout = new QVBoxLayout();
        contentLayout->setContentsMargins(4, 0, 0, 0);
        contentLayout->setSpacing(0);

        QLabel *label = new QLabel(QString::fromStdString(name));
        label->setFont(QFont(fontFamily, 9));
        label->setAlignment(Qt::AlignVCenter);
        label->setFixedHeight(11);
        label->setStyleSheet("color:" + QString::fromStdString(Theme::style.text) + "; font-weight: 600;");
        contentLayout->addWidget(label);

        QLabel *domain = new QLabel(QString::fromStdString(url));
        domain->setAlignment(Qt::AlignVCenter);
        domain->setFont(QFont(fontFamily, 7));
        domain->setFixedHeight(9);
        domain->setStyleSheet("color:" + QString::fromStdString(Theme::style.textHover) + "; font-weight: 600;");
        contentLayout->addWidget(domain);

        return contentLayout;
    }

    void setupPreview(const std::string &name)
    {
        previewWidget = new Preview(nullptr);
        previewWidget->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
        previewWidget->setAttribute(Qt::WA_StyledBackground, true);
        previewWidget->setFixedSize(155, 135);
        previewWidget->setContentsMargins(2, 2, 2, 2);

        opacityEffect = new QGraphicsOpacityEffect(previewWidget);
        opacityEffect->setOpacity(0.0);
        previewWidget->setGraphicsEffect(opacityEffect);

        previewWidget->hide();

        QVBoxLayout *previewLayout = new QVBoxLayout(previewWidget);
        previewLayout->setContentsMargins(0, 0, 0, 0);
        previewLayout->setAlignment(Qt::AlignTop);
        previewLayout->setSpacing(0);

        static QString fontFamily = loadFontFamily();

        QLabel *previewTitle = new QLabel("Apple");
        previewTitle->setContentsMargins(4, 4, 0, 0);
        previewTitle->setFont(QFont(fontFamily, 10));
        previewTitle->setStyleSheet("color:" + QString::fromStdString(Theme::style.text) + "; font-weight: 600;");
        previewLayout->addWidget(previewTitle);

        QLabel *domain = new QLabel("apple.com");
        domain->setContentsMargins(4, 0, 0, 0);
        domain->setFont(QFont(fontFamily, 9));
        domain->setStyleSheet("color:" + QString::fromStdString(Theme::style.textHover) + ";");
        previewLayout->addWidget(domain);

        Widget *sitePreviewWidget = new Widget();
        sitePreviewWidget->setContentsMargins(0, 0, 0, 0);
        sitePreviewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sitePreviewWidget->setMinimumSize(0, 0);
        sitePreviewWidget->setStyleSheet("background-color: " + QString::fromStdString(Theme::style.primary) + ";");
        sitePreviewWidget->setAttribute(Qt::WA_TranslucentBackground);

        QHBoxLayout *sitePreviewLayout = new QHBoxLayout(sitePreviewWidget);
        sitePreviewLayout->setContentsMargins(0, 0, 0, 0);
        sitePreviewLayout->setSpacing(0);

        Image *previewImage = new Image(":/images/apple.png");
        sitePreviewLayout->addWidget(previewImage);

        previewLayout->addWidget(sitePreviewWidget);

        QLabel *usage = new QLabel("Memory Usage: 259 MB");
        usage->setContentsMargins(4, 4, 0, 4);
        usage->setFont(QFont(fontFamily, 8));
        usage->setStyleSheet("color:" + QString::fromStdString(Theme::style.textHover) + "; font-weight: 600;");
        previewLayout->addWidget(usage);

        previewWidget->installEventFilter(this);

        hidePreviewTimer.setSingleShot(true);
        connect(&hidePreviewTimer, &QTimer::timeout, this, &Tab::hidePreview);
    }

    void setupAnimations()
    {
        showAnimation = new QPropertyAnimation(opacityEffect, "opacity");
        showAnimation->setDuration(200);
        showAnimation->setStartValue(0.0);
        showAnimation->setEndValue(1.0);
        showAnimation->setEasingCurve(QEasingCurve::OutCubic);

        hideAnimation = new QPropertyAnimation(opacityEffect, "opacity");
        hideAnimation->setDuration(150);
        hideAnimation->setStartValue(1.0);
        hideAnimation->setEndValue(0.0);
        hideAnimation->setEasingCurve(QEasingCurve::InCubic);

        connect(hideAnimation, &QPropertyAnimation::finished, [this]()
                {
            if (previewWidget && opacityEffect->opacity() == 0.0) {
                previewWidget->hide();
            } });
    }

    void setupLayout()
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(contentWidget);
    }

    static QString loadFontFamily()
    {
        static QString family;
        if (family.isEmpty())
        {
            int id = QFontDatabase::addApplicationFont(":/fonts/nunito.ttf");
            family = QFontDatabase::applicationFontFamilies(id).at(0);
        }
        return family;
    }

    void showPreview()
    {
        if (!previewWidget || previewWidget->isVisible())
            return;

        if (hideAnimation->state() == QAbstractAnimation::Running)
        {
            hideAnimation->stop();
        }

        QPoint globalPos = mapToGlobal(QPoint(0, height()));
        QScreen *screen = QApplication::screenAt(globalPos);

        if (screen)
        {
            QRect screenGeometry = screen->availableGeometry();

            if (globalPos.y() + previewWidget->height() > screenGeometry.bottom())
            {
                globalPos = mapToGlobal(QPoint(0, -previewWidget->height()));
            }

            if (globalPos.x() + previewWidget->width() > screenGeometry.right())
            {
                globalPos.setX((screenGeometry.right() - previewWidget->width()));
            }

            if (globalPos.x() < screenGeometry.left())
            {
                globalPos.setX(screenGeometry.left());
            }
        }

        previewWidget->move(globalPos.x(), globalPos.y());
        previewWidget->show();
        showAnimation->start();
    }

    void hidePreview()
    {
        if (previewWidget && previewWidget->isVisible())
        {
            if (showAnimation->state() == QAbstractAnimation::Running)
            {
                showAnimation->stop();
            }
            hideAnimation->start();
        }
    }
};