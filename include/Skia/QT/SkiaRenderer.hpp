#pragma once

#include "Headers/Global.hpp"
#include "Headers/SkiaWidgets.hpp"

class SkiaRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
 
public:
    explicit SkiaRenderer(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    virtual void onInit() = 0;
    virtual void onRender() = 0;
    virtual void onResize(int w, int h) = 0;

    SkCanvas *canvas;
private:
    int width;
    int height;
    
    bool isInit = true;

    sk_sp<GrDirectContext> grContext;
    sk_sp<SkSurface> surface;
};
