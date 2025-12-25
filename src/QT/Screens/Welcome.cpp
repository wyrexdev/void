#include "QT/Screens/Welcome.hpp"

namespace QT
{
    namespace Screens
    {
        void Welcome::init(QVBoxLayout *mainLayout)
        {
            welcomeText->setText("Welcome To Void Browser");
            welcomeText->setStyleSheet("color: #ffffff;");
            welcomeText->setFont(QFont(nunito, 80));
            mainLayout->addWidget(welcomeText);

            welcomeDescText->setText("We will adjusting the settings for better performance and security");
            welcomeDescText->setStyleSheet("color: #ffffff;");
            welcomeDescText->setFont(QFont(nunito, 50));
            mainLayout->addWidget(welcomeDescText);
        }
    } // namespace Screens
} // namespace QT
