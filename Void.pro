QT += widgets svg network concurrent

TARGET = Void
TEMPLATE = app

CONFIG += c++20
CONFIG += warn_on
CONFIG += qt

# Recursively find all .cpp files in src/ and its subdirectories
SOURCES = $$files(src/*.cpp, true) \
          $$files(src/*/*.cpp, true) \
          $$files(src/*/*/*.cpp, true)

# Recursively find all .h and .hpp files in include/ and its subdirectories
HEADERS = $$files(include/*.h, true) \
          $$files(include/*.hpp, true) \
          $$files(include/*/*.h, true) \
          $$files(include/*/*.hpp, true) \
          $$files(include/*/*/*.h, true) \
          $$files(include/*/*/*.hpp, true)

RESOURCES += resources.qrc

# Build directory
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build

# Compiler flags
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic

# Include paths
INCLUDEPATH += include
INCLUDEPATH += src

# For better debugging (optional)
# CONFIG += debug
# QMAKE_CXXFLAGS_DEBUG += -g -O0

# For release builds (optional)
# CONFIG += release
# QMAKE_CXXFLAGS_RELEASE += -O3

# Additional libraries if needed
# LIBS += -lssl -lcrypto

# Installation paths (optional)
# target.path = /usr/local/bin
# INSTALLS += target

# Print found files for debugging
message("Found source files: $$SOURCES")
message("Found header files: $$HEADERS")