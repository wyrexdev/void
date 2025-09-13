#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

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
#include <QResizeEvent>

#include "Widget/Widget.hpp"
#include "Utils/Theme.hpp"

class Image : public Widget
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal borderRadius READ borderRadius WRITE setBorderRadius)

public:
    enum SizeMode {
        Fixed,
        AutoAspectRatio,
        Expanding,
        Maximum
    };
    
    explicit Image(const QString &url = "", int w = 0, int h = 0, int r = 0, QWidget *parent = nullptr)
        : Widget(parent), m_scale(1.0), m_opacity(1.0), m_borderRadius(r), m_loading(false),
          m_requestedWidth(w), m_requestedHeight(h), m_sizeMode(AutoAspectRatio)
    {
        setupUI();
        setupEffects();
        
        if (!url.isEmpty()) {
            loadImage(url);
        }
        
        updateSizePolicy();
    }

    ~Image() {
        if (m_loadingTimer) {
            m_loadingTimer->stop();
            delete m_loadingTimer;
        }
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
    
    void setSizeMode(SizeMode mode) {
        if (m_sizeMode != mode) {
            m_sizeMode = mode;
            updateSizePolicy();
            updatePixmap();
        }
    }
    
    SizeMode sizeMode() const { return m_sizeMode; }
    
    void setImage(const QString &url) {
        loadImage(url);
    }
    
    void setFixedSize(int w, int h) {
        m_requestedWidth = w;
        m_requestedHeight = h;
        QWidget::setFixedSize(w, h);
        updatePixmap();
    }
    
    void setMinimumSize(int w, int h) {
        m_requestedWidth = w;
        m_requestedHeight = h;
        QWidget::setMinimumSize(w, h);
    }
    
    void setMaximumSize(int w, int h) {
        m_requestedWidth = w;
        m_requestedHeight = h;
        QWidget::setMaximumSize(w, h);
    }

signals:
    void clicked();
    void loadingStarted();
    void loadingFinished();
    void loadingFailed();

protected:
    void enterEvent(QEnterEvent *event) override {
        QWidget::enterEvent(event);
        if (!m_loading && !m_pixmap.isNull()) {
            animateHover(true);
        }
    }

    void leaveEvent(QEvent *event) override {
        QWidget::leaveEvent(event);
        if (!m_loading && !m_pixmap.isNull()) {
            animateHover(false);
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && !m_loading && !m_pixmap.isNull()) {
            animateClick();
            emit clicked();
        }
        QWidget::mousePressEvent(event);
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        if (!painter.isActive()) {
            return;
        }
        
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
        } else if (!m_originalPixmap.isNull()) {
            QTransform transform;
            transform.translate(width() / 2, height() / 2);
            transform.scale(m_scale, m_scale);
            transform.translate(-width() / 2, -height() / 2);
            painter.setTransform(transform);

            if (m_sizeMode == AutoAspectRatio) {
                QPixmap scaled = m_originalPixmap.scaled(
                    size(), 
                    Qt::KeepAspectRatio, 
                    Qt::SmoothTransformation
                );
                QRect targetRect(
                    (width() - scaled.width()) / 2,
                    (height() - scaled.height()) / 2,
                    scaled.width(),
                    scaled.height()
                );
                
                QPainterPath path;
                path.addRoundedRect(targetRect, m_borderRadius, m_borderRadius);
                painter.setClipPath(path);
                painter.drawPixmap(targetRect, scaled);
            } else {
                QPainterPath path;
                path.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
                painter.setClipPath(path);
                painter.drawPixmap(rect(), m_pixmap);
            }
        } else {
            painter.setBrush(QColor(240, 240, 240));
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(rect(), m_borderRadius, m_borderRadius);
            
            painter.setPen(QColor(180, 180, 180));
            painter.drawText(rect(), Qt::AlignCenter, "No Image");
        }
    }
    
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        if (m_sizeMode == AutoAspectRatio && !m_originalPixmap.isNull()) {
            update();
        } else if (!m_originalPixmap.isNull()) {
            updatePixmap();
        }
    }
    
    QSize sizeHint() const override {
        if (m_requestedWidth > 0 && m_requestedHeight > 0) {
            return QSize(m_requestedWidth, m_requestedHeight);
        }
        
        if (!m_originalPixmap.isNull()) {
            return m_originalPixmap.size();
        }
        
        return QSize(100, 100);
    }

private:
    void setupUI() {
        setAttribute(Qt::WA_TranslucentBackground);
        setMouseTracking(true);
    }

    void setupEffects() {
        setShadowEnabled(true);
    }
    
    void updateSizePolicy() {
        switch(m_sizeMode) {
            case Fixed:
                if (m_requestedWidth > 0 && m_requestedHeight > 0) {
                    setFixedSize(m_requestedWidth, m_requestedHeight);
                }
                setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                break;
            case AutoAspectRatio:
                setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                break;
            case Expanding:
                if (m_requestedWidth > 0 && m_requestedHeight > 0) {
                    setMinimumSize(m_requestedWidth, m_requestedHeight);
                }
                setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                break;
            case Maximum:
                if (m_requestedWidth > 0 && m_requestedHeight > 0) {
                    setMaximumSize(m_requestedWidth, m_requestedHeight);
                }
                setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                break;
        }
        update();
    }
    
    void updatePixmap() {
        if (m_originalPixmap.isNull()) return;
        
        if (m_sizeMode == AutoAspectRatio) {
            m_pixmap = QPixmap();
        } else {
            QSize targetSize = size();
            if (m_sizeMode == Fixed && m_requestedWidth > 0 && m_requestedHeight > 0) {
                targetSize = QSize(m_requestedWidth, m_requestedHeight);
            }
            
            QPixmap scaled = m_originalPixmap.scaled(
                targetSize, 
                Qt::IgnoreAspectRatio, 
                Qt::SmoothTransformation
            );
            
            QPixmap rounded(targetSize);
            rounded.fill(Qt::transparent);
            
            QPainter painter(&rounded);
            if (painter.isActive()) {
                painter.setRenderHint(QPainter::Antialiasing);
                
                QPainterPath path;
                path.addRoundedRect(QRect(0, 0, targetSize.width(), targetSize.height()), 
                                   m_borderRadius, m_borderRadius);
                painter.setClipPath(path);
                painter.drawPixmap(0, 0, scaled);
                
                painter.end();
                m_pixmap = rounded;
            } else {
                m_pixmap = scaled;
            }
        }
        
        update();
    }

    void loadImage(const QString &url) {
        m_loading = true;
        emit loadingStarted();
        update();
        
        if (url.startsWith(":/")) {
            loadFromResources(url);
            return;
        }
        
        if (!m_manager) {
            m_manager = new QNetworkAccessManager(this);
            connect(m_manager, &QNetworkAccessManager::finished, 
                    this, &Image::onImageDownloaded);
        }
        
        QNetworkRequest request(url);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, 
                           QNetworkRequest::NoLessSafeRedirectPolicy);
        
        m_manager->get(request);

        if (!m_loadingTimer) {
            m_loadingTimer = new QTimer(this);
            connect(m_loadingTimer, &QTimer::timeout, this, [this]() { update(); });
        }
        m_loadingTimer->start(50);
    }
    
    void loadFromResources(const QString &url) {
        QPixmap pixmap;
        if (pixmap.load(url)) {
            processLoadedPixmap(pixmap);
        } else {
            QSvgRenderer renderer(url);
            if (renderer.isValid()) {
                QSize imageSize;
                if (m_requestedWidth > 0 && m_requestedHeight > 0) {
                    imageSize = QSize(m_requestedWidth, m_requestedHeight);
                } else {
                    QSize defaultSize = renderer.defaultSize();
                    if (defaultSize.isEmpty()) {
                        imageSize = QSize(100, 100);
                    } else {
                        imageSize = defaultSize;
                    }
                }
                
                QPixmap svgPixmap(imageSize);
                svgPixmap.fill(Qt::transparent);
                
                QPainter painter(&svgPixmap);
                if (painter.isActive()) {
                    painter.setRenderHint(QPainter::Antialiasing);
                    renderer.render(&painter, QRect(0, 0, imageSize.width(), imageSize.height()));
                    painter.end();
                    
                    processLoadedPixmap(svgPixmap);
                } else {
                    handleLoadingFailed();
                }
            } else {
                handleLoadingFailed();
            }
        }
    }
    
    void processLoadedPixmap(const QPixmap &pixmap) {
        m_originalPixmap = pixmap;
        m_loading = false;
        
        if (m_requestedWidth <= 0 || m_requestedHeight <= 0) {
            m_requestedWidth = pixmap.width();
            m_requestedHeight = pixmap.height();
            updateSizePolicy();
        }
        
        updatePixmap();
        emit loadingFinished();
    }
    
    void handleLoadingFailed() {
        m_loading = false;
        if (m_loadingTimer) {
            m_loadingTimer->stop();
        }
        emit loadingFailed();
        update();
    }

    void animateHover(bool enter) {
        QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
        
        QPropertyAnimation* scaleAnim = new QPropertyAnimation(this, "scale");
        scaleAnim->setDuration(200);
        scaleAnim->setStartValue(m_scale);
        scaleAnim->setEndValue(enter ? m_hoverScale : 1.0);
        scaleAnim->setEasingCurve(QEasingCurve::OutCubic);
        
        QPropertyAnimation* opacityAnim = new QPropertyAnimation(this, "opacity");
        opacityAnim->setDuration(200);
        opacityAnim->setStartValue(m_opacity);
        opacityAnim->setEndValue(enter ? 0.9 : 1.0);
        opacityAnim->setEasingCurve(QEasingCurve::OutCubic);

        group->addAnimation(scaleAnim);
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
            releaseAnim->setEndValue(1.0);
            releaseAnim->setEasingCurve(QEasingCurve::InOutQuad);
            releaseAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });
        
        clickAnim->start(QAbstractAnimation::DeleteWhenStopped);
    }

private slots:
    void onImageDownloaded(QNetworkReply *reply) {
        m_loading = false;
        
        if (m_loadingTimer) {
            m_loadingTimer->stop();
        }

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            
            if (pixmap.loadFromData(imageData)) {
                processLoadedPixmap(pixmap);
            } else {
                QSvgRenderer renderer(imageData);
                if (renderer.isValid()) {
                    QSize imageSize;
                    if (m_requestedWidth > 0 && m_requestedHeight > 0) {
                        imageSize = QSize(m_requestedWidth, m_requestedHeight);
                    } else {
                        imageSize = renderer.defaultSize();
                        if (imageSize.isEmpty()) {
                            imageSize = QSize(100, 100);
                        }
                    }
                    
                    QPixmap svgPixmap(imageSize);
                    svgPixmap.fill(Qt::transparent);
                    
                    QPainter painter(&svgPixmap);
                    if (painter.isActive()) {
                        painter.setRenderHint(QPainter::Antialiasing);
                        renderer.render(&painter, QRect(0, 0, imageSize.width(), imageSize.height()));
                        painter.end();
                        
                        processLoadedPixmap(svgPixmap);
                    } else {
                        handleLoadingFailed();
                    }
                } else {
                    handleLoadingFailed();
                }
            }
        } else {
            handleLoadingFailed();
        }
        
        reply->deleteLater();
    }

private:
    QPixmap m_pixmap;
    QPixmap m_originalPixmap;
    qreal m_scale;
    qreal m_opacity;
    qreal m_borderRadius;
    qreal m_hoverScale = 1.05;
    bool m_loading;
    int m_requestedWidth;
    int m_requestedHeight;
    SizeMode m_sizeMode;

    QNetworkAccessManager* m_manager = nullptr;
    QTimer* m_loadingTimer = nullptr;
    QGraphicsDropShadowEffect* m_shadowEffect = nullptr;
};