#pragma once

// ----- QT Headers ----- //
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QLineEdit>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QSvgRenderer>
#include <QResizeEvent>
#include <QVariantAnimation>
#include <QGraphicsEffect>
#include <QFile>
#include <QSpacerItem>
#include <QEnterEvent>
#include <functional>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
// ----- QT Headers End ----- //

// ----- Skia Headers ----- //
#include "include/core/SkCanvas.h"
#include "include/core/SkStream.h"
#include "include/core/SkPaint.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColor.h"
#include "include/core/SkRect.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkFontMgr.h"

#include "include/ports/SkFontMgr_fontconfig.h"
#include "include/ports/SkFontScanner_Fontations.h"
#include "include/ports/SkFontScanner_FreeType.h"

#include "gpu/ganesh/gl/GrGLDirectContext.h"

#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"

#include "include/core/SkTextBlob.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkImage.h"
#include "include/core/SkData.h"
#include "include/codec/SkCodec.h"
#include "include/core/SkBitmap.h"
// ----- Skia Headers End ----- //

// ----- Math Headers ----- //
#include "Skia/Math/Vec2/Vec2.hpp"
#include "Skia/Math/Vec3/Vec3.hpp"
// ----- Math Headers End ----- //

// ----- Global Headers ----- //
#include <vector>
#include <iostream>
#include <malloc.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
// ----- Global Headers End ----- //

// ----- System Headers ----- //
#include "System/Screen.hpp"
// ----- System Headers End ----- //