#pragma once

#include <QObject>

namespace Signals
{
    class Skia : public QObject
    {
        Q_OBJECT
    public:
        explicit Skia(QObject *parent = nullptr) : QObject(parent) {}

        static void update() {
            emit instance()->updateSkia();
        }

        static Skia *instance()
        {
            static Skia inst;
            return &inst;
        }
    signals:
        void updateSkia();
    };
} // namespace Signals
