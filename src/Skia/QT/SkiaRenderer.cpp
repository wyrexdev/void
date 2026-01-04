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
    rectPaint.setColor(SkColorSetRGB(17, 17, 17));

    canvas->drawRoundRect(
        SkRect::MakeXYWH(0, 0, width, height),
        20, 20,
        rectPaint);

    // render area
    onRender();
    // end render area

    grContext->flushAndSubmit();
}

void SkiaRenderer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPointF pos = event->position();
        onMouseDown(pos.x(), pos.y());
    }
}

void SkiaRenderer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPointF pos = event->position();
        onMouseUp(pos.x(), pos.y());
    }
}

void SkiaRenderer::setURL(std::string u)
{
    url = u;
}

std::string SkiaRenderer::getURL()
{
    return url;
}

void SkiaRenderer::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();

    scrollY -= delta * 0.5f;
    clampScroll();

    std::cout
        << "viewport=" << Skia::Viewport::height
        << " content=" << totalHeight
        << " min=" << (Skia::Viewport::height - totalHeight)
        << std::endl;

    update();
}

void SkiaRenderer::clampScroll()
{
    float viewportHeight = Skia::Viewport::height;

    if (totalHeight <= viewportHeight)
    {
        scrollY = 0;
        return;
    }

    float maxScroll = 0;
    float minScroll = viewportHeight - totalHeight;

    if (scrollY > maxScroll)
        scrollY = maxScroll;
    if (scrollY < minScroll)
        scrollY = minScroll;
}
