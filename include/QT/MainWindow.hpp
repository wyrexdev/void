#pragma once

#include <QMainWindow>
#include <QResizeEvent>
#include <iostream>

#include "Engine/Engine.hpp"

#include "Skia/Canvas/Viewport.hpp"

#include "QT/Widget/Layouts/Nav.hpp"
#include "QT/Widget/Layouts/LeftSideBar.hpp"
#include "QT/Widget/Layouts/RightSideBar.hpp"
#include "QT/Widget/URLPreview/URLPreview.hpp"
#include "QT/Screens/Welcome.hpp"

#include "Utils/QT/History.hpp"
#include "Utils/QT/Font.hpp"
#include "Utils/Math/UUID.hpp"
#include "Utils/Ram/Ram.hpp"

#include "Engine/Signals/URLPreviewSignal.hpp"

#include "System/Setup/Setup.hpp"

#include "Wire/Client/WireClient.hpp"

namespace QT
{
    class MainWindow : public QMainWindow
    {
    public:
        MainWindow();

    private:
        Widget *siteContentWidget;
        Screens::Welcome *welcomeScreen;

        bool isSetupNeeded = true;

    protected:
        void resizeEvent(QResizeEvent *event) override;
    };
} // namespace QT
