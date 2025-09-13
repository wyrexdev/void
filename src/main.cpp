#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QLineEdit>

#include "Widget/Layouts/Nav.hpp"
#include "Widget/Layouts/LeftSideBar.hpp"
#include "Widget/Layouts/RightSideBar.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.background) + ";");

    QWidget *centralWidget = new QWidget();
    window.setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 0, 10, 0);
    mainLayout->setSpacing(0);

    Nav *nav = new Nav();

    Nav::NItem item = {
        logo : "https://substackcdn.com/image/fetch/$s_!G1lk!,f_auto,q_auto:good,fl_progressive:steep/https%3A%2F%2Fsubstack-post-media.s3.amazonaws.com%2Fpublic%2Fimages%2F8ed3d547-94ff-48e1-9f20-8c14a7030a02_2000x2000.jpeg",
        name : "Apple",
        url : "apple.com"
    };

    nav->addItem(item);
    nav->addItem(item);
    nav->addItem(item);
    nav->addItem(item);

    mainLayout->addWidget(nav);

    QWidget *contentContainer = new QWidget();
    contentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout *contentLayout = new QHBoxLayout(contentContainer);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    LeftSideBar *leftSideBar = new LeftSideBar();
    leftSideBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    RightSideBar *rightSideBar = new RightSideBar();
    rightSideBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QWidget *sidebarContainer = new QWidget();
    sidebarContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarContainer);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(leftSideBar);
    sidebarLayout->addStretch();

    contentLayout->addWidget(sidebarContainer);

    static QString nano = Font::getNano();
    static QString nunito = Font::getNunito();

    QLabel *vLabel = new QLabel("V");
    vLabel->setStyleSheet("color: #ffffff;");
    vLabel->setFont(QFont(nano, 50));

    QLabel *oLabel = new QLabel("O");
    oLabel->setStyleSheet("color: #6078ff;");
    oLabel->setFont(QFont(nano, 50));

    QHBoxLayout *voLayout = new QHBoxLayout();
    voLayout->setAlignment(Qt::AlignCenter);
    voLayout->setSpacing(0);
    voLayout->setContentsMargins(0, 0, 0, 0);

    voLayout->addWidget(vLabel);
    voLayout->addWidget(oLabel);

    Widget *browserWidget = new Widget();
    browserWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *browserLayout = new QVBoxLayout(browserWidget);
    browserLayout->setAlignment(Qt::AlignCenter);
    browserLayout->addLayout(voLayout);

    browserLayout->setSpacing(10);

    QLineEdit *searchBar = new QLineEdit();
    searchBar->setFont(QFont(nunito, 11));
    searchBar->setPlaceholderText("Search on VOID");
    searchBar->setFixedSize(400, 45);
    searchBar->setFrame(false);
    searchBar->setAttribute(Qt::WA_MacShowFocusRect, 0); // For MAC
    int radius = searchBar->height() / 3;
    searchBar->setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.tab) + ";" +
        "border: 2px solid " + QString::fromStdString(Theme::style.primary) + ";" +
        "border-radius: " + QString::number(radius) + ";" +
        "padding: 10px;"
        "padding-left: 20px;"
        "padding-right: 20px;"
        "font-weight: 600;"
        "color: #ffffff;"
    );
    browserLayout->addWidget(searchBar);

    contentLayout->addWidget(browserWidget);

    RightSideBar::RSItem netflix = {
        icon : "https://images.ctfassets.net/4cd45et68cgf/Rx83JoRDMkYNlMC9MKzcB/2b14d5a59fc3937afd3f03191e19502d/Netflix-Symbol.png?w=700&h=456",
        url : "https://netflix.com"
    };
    rightSideBar->addItem(netflix);

    RightSideBar::RSItem apple = {
        icon : "https://substackcdn.com/image/fetch/$s_!G1lk!,f_auto,q_auto:good,fl_progressive:steep/https%3A%2F%2Fsubstack-post-media.s3.amazonaws.com%2Fpublic%2Fimages%2F8ed3d547-94ff-48e1-9f20-8c14a7030a02_2000x2000.jpeg",
        url : "https://apple.com"
    };
    rightSideBar->addItem(apple);

    RightSideBar::RSItem x = {
        icon : "https://upload.wikimedia.org/wikipedia/commons/thumb/b/b7/X_logo.jpg/250px-X_logo.jpg",
        url : "https://x.com"
    };
    rightSideBar->addItem(x);

    contentLayout->addWidget(rightSideBar);

    mainLayout->addWidget(contentContainer);

    window.setWindowTitle("Void Browser");
    window.showMaximized();

    return app.exec();
}