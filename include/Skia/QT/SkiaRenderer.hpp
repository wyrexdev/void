#pragma once

#include "Headers/Global.hpp"
#include "Headers/SkiaWidgets.hpp"

#include "Skia/Canvas/Viewport.hpp"

class SkiaRenderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit SkiaRenderer(QWidget *parent = nullptr);

    void setURL(std::string url);
    std::string getURL();

    float scrollY;

protected:
    SkCanvas *canvas;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    virtual void onInit() = 0;
    virtual void onRender() = 0;
    virtual void onResize(int w, int h) = 0;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void onMouseDown(float x, float y) {}
    virtual void onMouseUp(float x, float y) {}

    void wheelEvent(QWheelEvent *event) override;

    float totalWidth;
    float totalHeight;
private:
    int width;
    int height;

    bool isInit = true;

    sk_sp<GrDirectContext> grContext;
    sk_sp<SkSurface> surface;

    std::string url;

    void clampScroll();
};
