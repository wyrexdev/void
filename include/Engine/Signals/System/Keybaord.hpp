#pragma once

#include <QObject>

namespace Signals
{
    namespace System
    {
        class Keyboard : public QObject
        {
            Q_OBJECT
        public:
            explicit Keyboard(QObject *parent = nullptr) : QObject(parent) {}

            static void send(char c, std::string k)
            {
                emit instance()->onKeyPress(c, k);
            }

            static Keyboard *instance()
            {
                static Keyboard inst;
                return &inst;
            }
        signals:
            void onKeyPress(char c, std::string k);
        };
    } // namespace System

} // namespace Signals
