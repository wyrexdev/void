#pragma once

#include "Headers/Global.hpp"

#include "System/Setup/Setup.hpp"

#include "Utils/QT/Font.hpp"

namespace QT
{
    namespace Screens
    {
        class Welcome : public QWidget
        {
            Q_OBJECT
        public:
            explicit Welcome(QWidget *parent = nullptr);

            void init(QVBoxLayout *mainLayout);
            void setupBottomArea();

            void animateString(QLabel *label, const QString &target, int intervalMs = 40);

            QWidget *getContentWidget();

        private:
            QTimer *timer;

            QWidget *contentWidget;
            QLabel *welcomeText;
            QLabel *welcomeDescText;

            QString nano;
            QString nunito;

            QVBoxLayout *mainLayout;
            
            // std::string wt = "WELCOME";
        };

    } // namespace Screens

} // namespace QT
