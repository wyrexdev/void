#pragma once

#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QSvgRenderer>

#include "Widget/Widget.hpp"

class ImageUrl : public Widget
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal borderRadius READ borderRadius WRITE setBorderRadius)

public:
    explicit ImageUrl(const QString &url, int w, int h, int r, QWidget *parent = nullptr)
        : Widget(parent), m_scale(1.0), m_opacity(1.0), m_borderRadius(r), m_loading(true)
    {
        setFixedSize(w, h);
        setupUI();
        setupEffects();
        loadImage(url);
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

    qreal borderRadius() const { return m_borderRadius; }
    void setBorderRadius(qreal radius) { 
        m_borderRadius = radius; 
        update();
    }

    void setHoverScale(qreal scale) { m_hoverScale = scale; }
    void setShadowEnabled(bool enabled) { 
        if (enabled && !m_shadowEffect) {
            m_shadowEffect = new QGraphicsDropShadowEffect(this);
            m_shadowEffect->setBlurRadius(20);
            m_shadowEffect->setColor(QColor(0, 0, 0, 80));
            m_shadowEffect->setOffset(0, 4);
            setGraphicsEffect(m_shadowEffect);
        } else if (!enabled && m_shadowEffect) {
            setGraphicsEffect(nullptr);
            delete m_shadowEffect;
            m_shadowEffect = nullptr;
        }
    }

signals:
    void clicked();
    void loadingFinished();
    void loadingFailed();

protected:
    void enterEvent(QEnterEvent *event) override {
        QWidget::enterEvent(event);
        if (!m_loading) {
            animateHover(true);
        }
    }

    void leaveEvent(QEvent *event) override {
        QWidget::leaveEvent(event);
        if (!m_loading) {
            animateHover(false);
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && !m_loading) {
            animateClick();
            emit clicked();
        }
        QWidget::mousePressEvent(event);
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setOpacity(m_opacity);
        painter.setRenderHint(QPainter::Antialiasing);

        if (m_loading) {
            painter.setBrush(QColor(220, 220, 220));
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(rect(), m_borderRadius, m_borderRadius);

            painter.setBrush(QColor(180, 180, 180));
            QTime time = QTime::currentTime();
            int angle = (time.msec() / 10) % 360;
            painter.drawPie(rect().adjusted(10, 10, -10, -10), angle * 16, 90 * 16);
        } else {
            QTransform transform;
            transform.translate(width() / 2, height() / 2);
            transform.scale(m_scale, m_scale);
            transform.translate(-width() / 2, -height() / 2);
            painter.setTransform(transform);

            if (!m_pixmap.isNull()) {
                QPainterPath path;
                path.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
                painter.setClipPath(path);
                painter.drawPixmap(rect(), m_pixmap);
            }
        }
    }

private:
    void setupUI() {
        setAttribute(Qt::WA_TranslucentBackground);
        setMouseTracking(true);
    }

    void setupEffects() {
        setShadowEnabled(true);
    }

    void loadImage(const QString &url) {
        m_manager = new QNetworkAccessManager(this);
        QNetworkRequest request(url);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, 
                           QNetworkRequest::NoLessSafeRedirectPolicy);
        
        connect(m_manager, &QNetworkAccessManager::finished, 
                this, &ImageUrl::onImageDownloaded);
        
        m_manager->get(request);

        m_loadingTimer = new QTimer(this);
        connect(m_loadingTimer, &QTimer::timeout, this, [this]() { update(); });
        m_loadingTimer->start(50);
    }

    void animateHover(bool enter) {
        QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
        
        QPropertyAnimation* opacityAnim = new QPropertyAnimation(this, "opacity");
        opacityAnim->setDuration(200);
        opacityAnim->setStartValue(m_opacity);
        opacityAnim->setEndValue(enter ? 0.9 : 1.0);
        opacityAnim->setEasingCurve(QEasingCurve::OutCubic);

        group->addAnimation(opacityAnim);
        group->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void animateClick() {
        QPropertyAnimation* clickAnim = new QPropertyAnimation(this, "scale");
        clickAnim->setDuration(100);
        clickAnim->setStartValue(m_scale);
        clickAnim->setEndValue(0.95);
        clickAnim->setEasingCurve(QEasingCurve::InOutQuad);
        
        connect(clickAnim, &QPropertyAnimation::finished, this, [this]() {
            QPropertyAnimation* releaseAnim = new QPropertyAnimation(this, "scale");
            releaseAnim->setDuration(100);
            releaseAnim->setStartValue(0.95);
            releaseAnim->setEndValue(m_hoverScale);
            releaseAnim->setEasingCurve(QEasingCurve::InOutQuad);
            releaseAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });
        
        clickAnim->start(QAbstractAnimation::DeleteWhenStopped);
    }

private slots:
    void onImageDownloaded(QNetworkReply *reply) {
        m_loadingTimer->stop();
        m_loading = false;

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            
            if (pixmap.loadFromData(imageData)) {
                QPixmap rounded(size());
                rounded.fill(Qt::transparent);
                
                QPainter painter(&rounded);
                painter.setRenderHint(QPainter::Antialiasing);
                
                QPainterPath path;
                path.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
                painter.setClipPath(path);
                painter.drawPixmap(rect(), pixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
                
                m_pixmap = rounded;
                emit loadingFinished();
            } else {
                QSvgRenderer renderer(imageData);
                if (renderer.isValid()) {
                    QPixmap svgPixmap(size());
                    svgPixmap.fill(Qt::transparent);
                    
                    QPainter painter(&svgPixmap);
                    painter.setRenderHint(QPainter::Antialiasing);
                    renderer.render(&painter, rect());
                    
                    m_pixmap = svgPixmap;
                    emit loadingFinished();
                } else {
                    emit loadingFailed();
                }
            }
        } else {
            emit loadingFailed();
        }
        
        reply->deleteLater();
        update();
    }

private:
    QPixmap m_pixmap;
    qreal m_scale;
    qreal m_opacity;
    qreal m_borderRadius;
    qreal m_hoverScale = 1.08;
    bool m_loading;

    QNetworkAccessManager* m_manager = nullptr;
    QTimer* m_loadingTimer = nullptr;
    QGraphicsDropShadowEffect* m_shadowEffect = nullptr;
};