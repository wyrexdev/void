#pragma once

#include "Headers/Global.hpp"

#include "Utils/QT/Font.hpp"

namespace QT
{
    namespace Screens
    {
        class Welcome
        {
        public:
            void init(QVBoxLayout *mainLayout);

        private:
            // Labels
            QLabel *welcomeText = new QLabel();
            QLabel *welcomeDescText = new QLabel();

            // Fonts
            QString nano = Font::getNano();
            QString nunito = Font::getNunito();
        };
    } // namespace Screens

} // namespace QT
