#include <QVulkanWindowRenderer>
#include <QVulkanDeviceFunctions>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <QObject>

class VulkanRenderer : public QObject, public QVulkanWindowRenderer {
    Q_OBJECT
public:
    VulkanRenderer(QVulkanWindow *w) : m_window(w), m_devFuncs(nullptr) {}
private: 
    QVulkanWindow *m_window;
    QVulkanDeviceFunctions *m_devFuncs;
protected:
    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;
    void startNextFrame() override;
};