#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QSvgRenderer>
#include <QPainter>
#include <QFile>
#include <QLabel>
#include <QSpacerItem>
#include <QEnterEvent>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QParallelAnimationGroup>
#include <functional>

#include "Widget/Widget.hpp"

class SvgWidget : public Widget {
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    SvgWidget(const QString& path, const QString& c, int w = 32, int h = 32, QWidget* parent = nullptr)
        : Widget(parent), svgPath(path), originalColor(c), color(c), m_scale(1.0), m_opacity(1.0)
    {
        loadSvg();
        setFixedSize(w, h);
        setMouseTracking(true);
        setAttribute(Qt::WA_TranslucentBackground);
        setupEffects();
    }

    qreal scale() const { return m_scale; }
    void setScale(qreal scale) { 
        m_scale = scale; 
        update();
    }

    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal opacity) { 
        m_opacity = opacity; 
        update();
    }

    void setHoverColor(const QString& hColor) {
        hoverColor = hColor;
    }

    void setShadowEnabled(bool enabled) {
        if (enabled && !shadowEffect) {
            shadowEffect = new QGraphicsDropShadowEffect(this);
            shadowEffect->setBlurRadius(15);
            shadowEffect->setColor(QColor(0, 0, 0, 60));
            shadowEffect->setOffset(0, 3);
            setGraphicsEffect(shadowEffect);
        } else if (!enabled && shadowEffect) {
            setGraphicsEffect(nullptr);
            delete shadowEffect;
            shadowEffect = nullptr;
        }
    }

    void updateColor(const QString& newColor) {
        QByteArray svgData = originalSvgData;
        svgData.replace("currentColor", newColor.toUtf8());
        svgData.replace("#000000", newColor.toUtf8());
        svgData.replace("black", newColor.toUtf8());
        svgData.replace("fill=\"#000000\"", QString("fill=\"%1\"").arg(newColor).toUtf8());
        
        renderer.load(svgData);
        color = newColor;
        update();
    }

    void animateHover(bool enter) {
        QPropertyAnimation* scaleAnim = new QPropertyAnimation(this, "scale");
        scaleAnim->setDuration(200);
        scaleAnim->setStartValue(m_scale);
        scaleAnim->setEndValue(enter ? 1.15 : 1.0);
        scaleAnim->setEasingCurve(QEasingCurve::OutBack);
        
        QPropertyAnimation* opacityAnim = new QPropertyAnimation(this, "opacity");
        opacityAnim->setDuration(150);
        opacityAnim->setStartValue(m_opacity);
        opacityAnim->setEndValue(enter ? 0.9 : 1.0);
        opacityAnim->setEasingCurve(QEasingCurve::OutCubic);

        QParallelAnimationGroup* animationGroup = new QParallelAnimationGroup(this);
        animationGroup->addAnimation(scaleAnim);
        animationGroup->addAnimation(opacityAnim);
        
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void setOnClick(std::function<void()> callback) {
        onClickCallback = callback;
    }

protected:
    void enterEvent(QEnterEvent* event) override {
        QWidget::enterEvent(event);
        isHovered = true;
        updateColor(hoverColor);
        animateHover(true);
        
        if (onHoverEnterCallback) {
            onHoverEnterCallback();
        }
    }

    void leaveEvent(QEvent* event) override {
        QWidget::leaveEvent(event);
        isHovered = false;
        updateColor(originalColor);
        animateHover(false);
        
        if (onHoverLeaveCallback) {
            onHoverLeaveCallback();
        }
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && onClickCallback) {
            QPropertyAnimation* pressAnim = new QPropertyAnimation(this, "scale");
            pressAnim->setDuration(100);
            pressAnim->setStartValue(m_scale);
            pressAnim->setEndValue(0.9);
            pressAnim->setEasingCurve(QEasingCurve::InOutQuad);
            
            connect(pressAnim, &QPropertyAnimation::finished, this, [this]() {
                if (onClickCallback) {
                    onClickCallback();
                }
                
                QPropertyAnimation* releaseAnim = new QPropertyAnimation(this, "scale");
                releaseAnim->setDuration(100);
                releaseAnim->setStartValue(0.9);
                releaseAnim->setEndValue(isHovered ? 1.15 : 1.0);
                releaseAnim->setEasingCurve(QEasingCurve::InOutQuad);
                releaseAnim->start(QAbstractAnimation::DeleteWhenStopped);
            });
            
            pressAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }
        QWidget::mousePressEvent(event);
    }

    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setOpacity(m_opacity);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        
        QTransform transform;
        transform.translate(width() / 2, height() / 2);
        transform.scale(m_scale, m_scale);
        transform.translate(-width() / 2, -height() / 2);
        painter.setTransform(transform);
        
        renderer.render(&painter, rect());
    }

private:
    void loadSvg() {
        QFile file(svgPath);
        if (file.open(QIODevice::ReadOnly)) {
            originalSvgData = file.readAll();
            updateColor(originalColor);
        }
    }

    void setupEffects() {
        setShadowEnabled(true);
    }

private:
    QSvgRenderer renderer;
    QString svgPath;
    QString originalColor;
    QString hoverColor = "#ffffff";
    QString color = "#ffffff";
    QByteArray originalSvgData;
    bool isHovered = false;
    qreal m_scale;
    qreal m_opacity;
    QGraphicsDropShadowEffect* shadowEffect = nullptr;

    std::function<void()> onHoverEnterCallback;
    std::function<void()> onHoverLeaveCallback;
    std::function<void()> onClickCallback;
};