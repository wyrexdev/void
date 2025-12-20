#include "Skia/QT/SkiaRenderer.hpp"

SkiaRenderer::SkiaRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
}

void SkiaRenderer::initializeGL()
{
    initializeOpenGLFunctions();

    auto interface = GrGLMakeNativeInterface();
    grContext = GrDirectContexts::MakeGL(interface);

    if (!grContext)
    {
        qFatal("Skia GrDirectContext Cannot Create");
    }

    onInit();
}

void SkiaRenderer::resizeGL(int w, int h)
{
    if (!grContext)
        return;

    makeCurrent();

    width = w;
    height = h;
    
    const int dpr = devicePixelRatioF();
    const int fbW = w * dpr;
    const int fbH = h * dpr;

    surface.reset();

    GLint fbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);

    GrGLFramebufferInfo fbInfo;
    fbInfo.fFBOID = fbo;
    fbInfo.fFormat = GL_RGBA8;

    GrBackendRenderTarget backendRT =
        GrBackendRenderTargets::MakeGL(
            fbW,
            fbH,
            /*sampleCnt*/ 0,
            /*stencilBits*/ 0,
            fbInfo);

    SkSurfaceProps props(0, kUnknown_SkPixelGeometry);

    surface = SkSurfaces::WrapBackendRenderTarget(
        grContext.get(),
        backendRT,
        kBottomLeft_GrSurfaceOrigin,
        kRGBA_8888_SkColorType,
        nullptr,
        &props,
        nullptr,
        nullptr);

    onResize(w, h);
}

void SkiaRenderer::paintGL()
{
    if (!surface || !grContext)
        return;

    canvas = surface->getCanvas();
    canvas->clear(SkColorSetARGB(255, 17, 17, 17));

    SkPaint rectPaint;
    rectPaint.setAntiAlias(true);
    rectPaint.setColor(SkColorSetRGB(96, 120, 255));

    canvas->drawRoundRect(
        SkRect::MakeXYWH(0, 0, width, height),
        20, 20,
        rectPaint);

    // render area
    onRender();
    // end render area

    grContext->flush();
}
