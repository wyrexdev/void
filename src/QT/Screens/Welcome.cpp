#include "QT/Screens/Welcome.hpp"

namespace QT
{
    namespace Screens
    {
        Welcome::Welcome(QWidget *parent) : QWidget(parent)
        {
            nunito = Font::getNunito();

            timer = new QTimer(this);
        }

        void Welcome::init(QVBoxLayout *mainLayout)
        {
            this->mainLayout = mainLayout;

            contentWidget = new QWidget();
            contentWidget->setStyleSheet(
                "background-color: #161616;"
                "border-radius: 50px;");

            QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
            contentLayout->setAlignment(Qt::AlignCenter);
            contentLayout->setSpacing(20);

            QLabel *welcomeText = new QLabel();
            welcomeText->setText("");
            welcomeText->setFont(QFont(nunito, 35));
            welcomeText->setAlignment(Qt::AlignCenter);
            welcomeText->setStyleSheet("color: #fff;");
            contentLayout->addWidget(welcomeText);

            welcomeDescText = new QLabel();
            welcomeDescText->setMinimumHeight(200);
            welcomeDescText->setText("Setting up your browser for optimal performance and security");
            welcomeDescText->setFont(QFont(nunito, 20));
            welcomeDescText->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            welcomeDescText->setStyleSheet("color: #fff;");

            QWidget *descWrapper = new QWidget();
            QVBoxLayout *descLayout = new QVBoxLayout(descWrapper);
            descLayout->setContentsMargins(0, 0, 0, 0);
            descLayout->addWidget(welcomeDescText);

            contentLayout->addWidget(descWrapper);

            QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(welcomeDescText);
            welcomeDescText->setGraphicsEffect(opacityEffect);
            opacityEffect->setOpacity(0.0);

            QPoint finalPos = welcomeDescText->pos();
            QPoint startPos = finalPos + QPoint(0, 100);
            welcomeDescText->move(startPos);

            QPropertyAnimation *moveAnim = new QPropertyAnimation(welcomeDescText, "pos");
            moveAnim->setDuration(1500);
            moveAnim->setStartValue(startPos);
            moveAnim->setEndValue(finalPos);
            moveAnim->setEasingCurve(QEasingCurve::OutCubic);

            QPropertyAnimation *opacityAnim = new QPropertyAnimation(opacityEffect, "opacity");
            opacityAnim->setDuration(1500);
            opacityAnim->setStartValue(0.0);
            opacityAnim->setEndValue(1.0);
            opacityAnim->setEasingCurve(QEasingCurve::OutCubic);

            QParallelAnimationGroup *animGroup = new QParallelAnimationGroup(this);
            animGroup->addAnimation(moveAnim);
            animGroup->addAnimation(opacityAnim);
            animGroup->start(QAbstractAnimation::DeleteWhenStopped);

            mainLayout->addStretch();
            mainLayout->addWidget(contentWidget, 0, Qt::AlignCenter);
            mainLayout->addStretch();

            animateString(welcomeText, "WELCOME", 15);

            connect(timer, &QTimer::timeout, this, [=]() mutable
                    {
                        static int counter = 0;
                        int dotCount = counter % 4;

                        std::string dots(dotCount, '.');

                        welcomeDescText->setText(
                           ("Setting up your browser for optimal performance and security" + dots).c_str()
                        );

                        counter++; });

            timer->setInterval(400);

            QTimer::singleShot(1500, this, [=]()
                               { timer->start(); });
        }

        void Welcome::setupBottomArea()
        {
            QVBoxLayout *bottomArea = new QVBoxLayout();

            mainLayout->addLayout(bottomArea);
        }

        QWidget *Welcome::getContentWidget()
        {
            return contentWidget;
        }

        void Welcome::animateString(QLabel *label, const QString &target, int intervalMs)
        {
            if (target.isEmpty())
                return;

            QString finalText = target.toUpper();
            QString currentText(finalText.length(), 'A');

            int index = 0;
            QChar currentChar = 'A';

            QTimer *timer = new QTimer(label);

            QObject::connect(timer, &QTimer::timeout, label, [=]() mutable
                             {
        if (index >= finalText.length())
        {
            label->setText(finalText);
            timer->stop();
            timer->deleteLater();
            return;
        }

        currentText[index] = currentChar;
        label->setText(currentText);

        if (currentChar == finalText[index])
        {
            index++;
            currentChar = 'A';
        }
        else
        {
            currentChar = QChar(currentChar.unicode() + 1);
        } });

            timer->start(intervalMs);
        }

    } // namespace Screens
} // namespace QT
