#include "Skia/QT/SkiaRenderWidget.hpp"
#include <QDebug>

SkiaRenderWidget::SkiaRenderWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
}

void SkiaRenderWidget::initializeGL()
{
    initializeOpenGLFunctions();

    auto interface = GrGLMakeNativeInterface();
    grContext = GrDirectContexts::MakeGL(interface);

    if (!grContext)
    {
        qFatal("Skia GrDirectContext Cannot Create");
    }
}

void SkiaRenderWidget::resizeGL(int w, int h)
{
    if (!grContext)
        return;

    makeCurrent();

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
}

void SkiaRenderWidget::paintGL()
{
    if (!surface || !grContext)
        return;

    SkCanvas *canvas = surface->getCanvas();
    canvas->clear(SkColorSetRGB(0, 122, 255));

    SkPaint rectPaint;
    rectPaint.setAntiAlias(true);
    rectPaint.setColor(SkColorSetRGB(96, 120, 255));

    canvas->drawRoundRect(
        SkRect::MakeXYWH(50, 50, 400, 200),
        20, 20,
        rectPaint);

    auto scanner = SkFontScanner_Make_FreeType();

    sk_sp<SkFontMgr> fontMgr =
        SkFontMgr_New_FontConfig(nullptr, std::move(scanner));

    sk_sp<SkTypeface> typeface =
        fontMgr->makeFromFile("./fonts/nunito.ttf");

    if (!typeface)
    {
        qWarning() << "Font cannot load!";
        return;
    }

    SkFont font(typeface, 32);
    font.setEdging(SkFont::Edging::kSubpixelAntiAlias);

    SkPaint textPaint;
    textPaint.setAntiAlias(true);
    textPaint.setColor(SK_ColorWHITE);

    canvas->drawString(
        "VOID Browser",
        80,
        140,
        font,
        textPaint);

    grContext->flush();
}
