#include "QT/Screens/Welcome.hpp"

namespace QT
{
    namespace Screens
    {
        Welcome::Welcome(QWidget *parent) : QWidget(parent)
        {
        }

        void Welcome::init(QVBoxLayout *mainLayout)
        {
            contentWidget = new QWidget();
            contentWidget->setStyleSheet(
                "background-color: #1e1b1b;"
                "border-radius: 12px;");

            QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
            contentLayout->setAlignment(Qt::AlignCenter);
            contentLayout->setSpacing(20);

            QLabel *welcomeText = new QLabel();
            welcomeText->setText("Welcome To Void Browser");
            welcomeText->setFont(QFont(nunito, 45));
            welcomeText->setAlignment(Qt::AlignCenter);
            welcomeText->setStyleSheet("color: #fff;");
            contentLayout->addWidget(welcomeText);

            QLabel *welcomeDescText = new QLabel();
            welcomeDescText->setText("We will adjusting the settings for better performance and security");
            welcomeDescText->setFont(QFont(nunito, 30));
            welcomeDescText->setAlignment(Qt::AlignCenter);
            welcomeDescText->setStyleSheet("color: #fff;");
            contentLayout->addWidget(welcomeDescText);

            mainLayout->addStretch();
            mainLayout->addWidget(contentWidget, 0, Qt::AlignCenter);
            mainLayout->addStretch();
        }

        QWidget *Welcome::getContentWidget() {
            return contentWidget;
        }
        
    } // namespace Screens
} // namespace QT
