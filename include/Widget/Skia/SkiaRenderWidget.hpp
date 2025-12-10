#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "include/core/SkCanvas.h"
#include "include/core/SkStream.h"
#include "include/core/SkPaint.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColor.h"
#include "include/core/SkRect.h"

#include "include/ports/SkFontMgr_fontconfig.h"
#include "include/ports/SkFontScanner_Fontations.h"
#include "include/ports/SkFontScanner_FreeType.h"

#include "gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/ganesh/GrBackendSurface.h"

#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkFontMgr.h"

#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"

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
