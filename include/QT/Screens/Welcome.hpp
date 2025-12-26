#pragma once

#include "Headers/Global.hpp"

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

            QWidget *contentWidget;
        private:
            QLabel *welcomeText;
            QLabel *welcomeDescText;

            QString nano;
            QString nunito;
        };

    } // namespace Screens

} // namespace QT
