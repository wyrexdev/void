#pragma once

#include <QObject>

namespace Signals
{
    class Nav : public QObject
    {
        Q_OBJECT
    public:
        explicit Nav(QObject *parent = nullptr) : QObject(parent) {}

        static void updateCurrentTabHeap(std::string heap) {
            emit instance()->whenUpdateCurrentTabHeap(heap);
        }

        static Nav *instance()
        {
            static Nav inst;
            return &inst;
        }
    signals:
        void whenUpdateCurrentTabHeap(std::string heap);
    };
} // namespace Signals
