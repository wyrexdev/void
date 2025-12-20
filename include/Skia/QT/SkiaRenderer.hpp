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

    // virtual void init();
    // virtual void render();
    // virtual void resize(int w, int h);

private:
    bool isInit = true;

    sk_sp<GrDirectContext> grContext;
    sk_sp<SkSurface> surface;
    SkCanvas *canvas;

    Skia::TextRenderer *text;
};
