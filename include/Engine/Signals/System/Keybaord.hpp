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

            static void sendKeyDown(int key, std::string s, Qt::KeyboardModifiers mods)
            {
                emit instance() -> keyDown(key, s, mods);
            }

            static void sendTextInput(const std::string &utf8)
            {
                emit instance() -> textInput(utf8);
            }

            static Keyboard *instance()
            {
                static Keyboard inst;
                return &inst;
            }
        signals:
            void keyDown(int key, std::string s, Qt::KeyboardModifiers mods);
            void textInput(const std::string &utf8);
        };
    } // namespace System

} // namespace Signals
