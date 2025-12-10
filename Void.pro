QT += widgets svg network concurrent opengl openglwidgets
TARGET = Void
TEMPLATE = app

CONFIG += c++20 warn_on qt

SOURCES = $$files(src/*.cpp, true) \
          $$files(src/*/*.cpp, true) \
          $$files(src/*/*/*.cpp, true)

HEADERS = $$files(include/*.h, true) \
          $$files(include/*.hpp, true) \
          $$files(include/*/*.h, true) \
          $$files(include/*/*.hpp, true) \
          $$files(include/*/*/*.h, true) \
          $$files(include/*/*/*.hpp, true)

RESOURCES += resources.qrc

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build

QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
QMAKE_LFLAGS += -lstdc++

INCLUDEPATH += \
    include \
    src \
    /usr/include/freetype2 \
    /home/wyrex/opt/skia \
    /home/wyrex/opt/skia/include

LIBS += \
    /home/wyrex/opt/skia/out/Release/libskia.a \
    -lfontconfig \
    -lfreetype \
    -lexpat \
    -lz \
    -lpng \
    -ljpeg \
    -lwebp \
    -lwebpdemux \
    -lbrotlidec \
    -lbrotlicommon \
    -lbz2 \
    -lssl \
    -lcrypto \
    -lcurl \
    -lEGL \
    -lGLESv2 \
    -lpthread \
    -ldl \
    -lrt
