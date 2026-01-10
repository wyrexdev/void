#pragma once

#include <QObject>

namespace Signals
{
    namespace System
    {
        class Mouse : public QObject
        {
            Q_OBJECT
        public:
            explicit Mouse(QObject *parent = nullptr) : QObject(parent) {}

            static void click(float x, float y)
            {
                emit instance()->onClick(x, y);
            }

            static Mouse *instance()
            {
                static Mouse inst;
                return &inst;
            }
        signals:
            void onClick(float x, float y);
        };
    } // namespace System

} // namespace Signals
