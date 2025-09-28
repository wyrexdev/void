#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QLineEdit>

#include "Engine/Request/Fetcher.hpp"

#include "Widget/Layouts/Nav.hpp"
#include "Widget/Layouts/LeftSideBar.hpp"
#include "Widget/Layouts/RightSideBar.hpp"

#include "Utils/UUID.hpp"

#include "Engine/Parser/Html/Parser.hpp"
#include "Engine/Engine.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    Engine *engine = new Engine();
    engine->setRenderEngine(EngineTypes::OpenGL);

    QMainWindow window;
    window.setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.background) + ";");

    QWidget *centralWidget = new QWidget();
    window.setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 0, 10, 0);
    mainLayout->setSpacing(0);

    QWidget *contentContainer = new QWidget();
    contentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout *contentLayout = new QHBoxLayout(contentContainer);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    Nav *nav = new Nav();
    mainLayout->addWidget(nav);

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

    Widget *siteContentWidget = new Widget();
    siteContentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *siteContentLayout = new QVBoxLayout(siteContentWidget);
    siteContentWidget->hide();

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
        "color: #ffffff;");
    browserLayout->addWidget(searchBar);

    QString query = searchBar->text();
    QString url = "https://www.google.com/search?q=" + QString(QUrl::toPercentEncoding(query));

    Nav::NItem i = {
        UUID::Random(),
        ":/images/logo.png",
        "New Tab",
        "void"};
    nav->addItem(i);
    History::setCurrentTab(i.uuid);

    /*class VulkanWindow : public QVulkanWindow
    {
    public:
        QVulkanWindowRenderer *createRenderer() override
        {
            VulkanRenderer *vr = new VulkanRenderer(this);
            vulkanRenderer = vr;
            return vr;
        }
    };

    QVulkanInstance *inst = new QVulkanInstance();
    inst->create();

    QVulkanWindow *vulkanWindow = new VulkanWindow();
    vulkanWindow->setVulkanInstance(inst);*/

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    engine->init(siteContentWidget);
    
    QObject::connect(searchBar, &QLineEdit::returnPressed, [=]
                     {
        Nav::NItem i;
        i.name = "Google";
        i.logo = "https://upload.wikimedia.org/wikipedia/commons/thumb/c/c1/Google_%22G%22_logo.svg/1200px-Google_%22G%22_logo.svg.png";
        i.url = searchBar->text().toStdString();

        nav->updateItem(History::currenTab, i);

        QString query = searchBar->text();
        QString encoded = QUrl::toPercentEncoding(query);
        std::string url = "https://www.google.com/search?q=" + encoded.toStdString();

        Fetcher *fetcher = new Fetcher();
        std::string content = fetcher->get(url);

        engine->parse(content);

        browserWidget->hide();
        siteContentWidget->show();

        //QWidget *ct = QWidget::createWindowContainer(vulkanWindow);
        engine->addRenderLayout(siteContentLayout);

        searchBar->setText(""); });

    nav->plusIcon->setOnClick([=]
                              {
        Nav::NItem item = {
            UUID::Random(),
            ":/images/logo.png",
            "New Tab",
            "void"
        };

        nav->addItem(item);

        History::setCurrentTab(item.uuid); });

    contentLayout->addWidget(browserWidget);
    contentLayout->addWidget(siteContentWidget);

    RightSideBar::RSItem netflix = {
        "https://images.ctfassets.net/4cd45et68cgf/Rx83JoRDMkYNlMC9MKzcB/2b14d5a59fc3937afd3f03191e19502d/Netflix-Symbol.png?w=700&h=456",
        "https://netflix.com"};
    rightSideBar->addItem(netflix);

    RightSideBar::RSItem apple = {
        "https://substackcdn.com/image/fetch/$s_!G1lk!,f_auto,q_auto:good,fl_progressive:steep/https%3A%2F%2Fsubstack-post-media.s3.amazonaws.com%2Fpublic%2Fimages%2F8ed3d547-94ff-48e1-9f20-8c14a7030a02_2000x2000.jpeg",
        "https://apple.com"};
    rightSideBar->addItem(apple);

    RightSideBar::RSItem x = {
        "https://upload.wikimedia.org/wikipedia/commons/thumb/b/b7/X_logo.jpg/250px-X_logo.jpg",
        "https://x.com"};
    rightSideBar->addItem(x);

    contentLayout->addWidget(rightSideBar);

    mainLayout->addWidget(contentContainer);

    window.setWindowTitle("Void Browser");
    window.showMaximized();

    return app.exec();
}