#pragma once

#include <QHBoxLayout>
#include <QVariantAnimation>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <memory>

#include "Widget/Widget.hpp"
#include "Widget/Svg/SvgWidget.hpp"
#include "Utils/Theme.hpp"

class LSItem : public Widget
{
    Q_OBJECT

public:
    explicit LSItem(const std::string &iconPath, QWidget *parent = nullptr)
        : Widget(parent), m_rippleRadius(0), m_backgroundOpacity(0.0), m_isActive(false)
    {
        initializeUI(iconPath);
        setupAnimations();
        setHoverCursor(Qt::PointingHandCursor);

        auto shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setBlurRadius(15);
        shadowEffect->setColor(QColor(0, 0, 0, 40));
        shadowEffect->setOffset(0, 2);
        setGraphicsEffect(shadowEffect);
    }

    void setActive(bool active)
    {
        m_isActive = active;
        updateBackground();
    }

    bool isActive() const { return m_isActive; }

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        if (m_backgroundOpacity > 0.0)
        {
            QColor bgColor = QColor(QString::fromStdString(Theme::style.onSurface));
            bgColor.setAlphaF(m_backgroundOpacity * 0.16);
            painter.setBrush(bgColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(rect(), 12, 12);
        }

        if (m_rippleRadius > 0)
        {
            QColor rippleColor = QColor(QString::fromStdString(Theme::style.onSurface));
            rippleColor.setAlphaF(0.2 - (m_rippleRadius / 100.0) * 0.15);

            painter.setBrush(rippleColor);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(rect().center(), m_rippleRadius, m_rippleRadius);
        }

        if (m_isActive)
        {
            painter.setBrush(QColor(QString::fromStdString(Theme::style.onSurface)));
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(QRect(2, height() - 4, width() - 4, 2), 1, 1);
        }

        Widget::paintEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            auto clickAnimation = new QVariantAnimation(this);
            clickAnimation->setDuration(200);
            clickAnimation->setStartValue(0);
            clickAnimation->setEndValue(width() / 2);
            clickAnimation->setEasingCurve(QEasingCurve::OutQuad);

            connect(clickAnimation, &QVariantAnimation::valueChanged,
                    this, [this](const QVariant &value)
                    {
                m_rippleRadius = value.toInt();
                update(); });

            connect(clickAnimation, &QVariantAnimation::finished,
                    this, [this, clickAnimation]
                    {
                clickAnimation->deleteLater();
                emit clicked(); });

            clickAnimation->start();
        }
        Widget::mousePressEvent(event);
    }

private:
    void initializeUI(const std::string &iconPath)
    {
        setFixedSize(48, 48);
        setAttribute(Qt::WA_StyledBackground, true);
        setStyleSheet(QString::fromStdString(
            "border-radius: 12px;"
            "background-color: transparent;"
            "transition: all 0.3s ease;"));

        auto icon = std::make_unique<SvgWidget>(
            QString::fromStdString(iconPath),
            m_isActive ? QString::fromStdString(Theme::style.onSurface)
                       : QString::fromStdString(Theme::style.text),
            24, 24);

        m_iconWidget = icon.get();

        auto layout = std::make_unique<QHBoxLayout>(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setAlignment(Qt::AlignCenter);
        layout->addWidget(icon.release());

        setLayout(layout.release());
    }

    void setupAnimations()
    {
        setOnHoverEnter([this]
                        {
            if (m_isActive) return;
            
            auto animation = new QVariantAnimation(this);
            animation->setDuration(300);
            animation->setStartValue(m_backgroundOpacity);
            animation->setEndValue(1.0);
            animation->setEasingCurve(QEasingCurve::OutQuad);
            
            connect(animation, &QVariantAnimation::valueChanged, 
                    this, [this](const QVariant& value) {
                m_backgroundOpacity = value.toDouble();
                updateIconColor();
                update();
            });
            
            animation->start(QAbstractAnimation::DeleteWhenStopped); });

        setOnHoverLeave([this]
                        {
            if (m_isActive) return;
            
            auto animation = new QVariantAnimation(this);
            animation->setDuration(300);
            animation->setStartValue(m_backgroundOpacity);
            animation->setEndValue(0.0);
            animation->setEasingCurve(QEasingCurve::OutQuad);
            
            connect(animation, &QVariantAnimation::valueChanged, 
                    this, [this](const QVariant& value) {
                m_backgroundOpacity = value.toDouble();
                updateIconColor();
                update();
            });
            
            animation->start(QAbstractAnimation::DeleteWhenStopped); });
    }

    void updateBackground()
    {
        m_backgroundOpacity = m_isActive ? 1.0 : 0.0;
        updateIconColor();
        update();
    }

    void updateIconColor()
    {
        if (m_iconWidget)
        {
            QString color = m_isActive || m_backgroundOpacity > 0.5
                                ? QString::fromStdString(Theme::style.onSurface)
                                : QString::fromStdString(Theme::style.text);
        }
    }

private:
    int m_rippleRadius;
    double m_backgroundOpacity;
    bool m_isActive;
    SvgWidget *m_iconWidget;
};