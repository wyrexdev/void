#pragma once

#include "Headers/Global.hpp"

class SkiaRenderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
 
public:
    explicit SkiaRenderWidget(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    sk_sp<GrDirectContext> grContext;
    sk_sp<SkSurface> surface;
};
