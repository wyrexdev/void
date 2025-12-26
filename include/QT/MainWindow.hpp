#pragma once

#include <QMainWindow>
#include <QResizeEvent>
#include <iostream>

namespace QT
{
    class MainWindow : public QMainWindow
    {
    public:
        MainWindow();
    protected:
        void resizeEvent(QResizeEvent *event) override;
    };
} // namespace QT
