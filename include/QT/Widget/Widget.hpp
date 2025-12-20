#pragma once

#include "Headers/Global.hpp"

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr) : QWidget(parent) {
        setMouseTracking(true);
        setAttribute(Qt::WA_Hover, true);
    }

    void setOnHoverEnter(std::function<void()> callback) {
        onHoverEnterCallback = callback;
    }

    void setOnHoverLeave(std::function<void()> callback) {
        onHoverLeaveCallback = callback;
    }

    void setOnClick(std::function<void()> callback) {
        onClickCallback = callback;
    }

    void setOnDoubleClick(std::function<void()> callback) {
        onDoubleClickCallback = callback;
    }

    void setHoverCursor(QCursor cursor) {
        this->cursor = cursor;
    }

signals:
    void hoverEntered();
    void hoverLeft();
    void clicked();
    void doubleClicked();

protected:
    void enterEvent(QEnterEvent *event) override {
        QWidget::enterEvent(event);
        if (onHoverEnterCallback) {
            onHoverEnterCallback();
        }

        setCursor(cursor);

        emit hoverEntered();
    }

    void leaveEvent(QEvent *event) override {
        QWidget::leaveEvent(event);
        if (onHoverLeaveCallback) {
            onHoverLeaveCallback();
        }
        emit hoverLeft();
    }

    void mousePressEvent(QMouseEvent *event) override {
        QWidget::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
            if (onClickCallback) {
                onClickCallback();
            }
            emit clicked();
        }
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override {
        QWidget::mouseDoubleClickEvent(event);
        if (event->button() == Qt::LeftButton) {
            if (onDoubleClickCallback) {
                onDoubleClickCallback();
            }
            emit doubleClicked();
        }
    }

private:
    std::function<void()> onHoverEnterCallback;
    std::function<void()> onHoverLeaveCallback;
    std::function<void()> onClickCallback;
    std::function<void()> onDoubleClickCallback;

    QCursor cursor = Qt::ArrowCursor;
};