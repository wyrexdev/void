#include <QVulkanWindowRenderer>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QDebug>

class VulkanRenderer : public QVulkanWindowRenderer {
public:
    VulkanRenderer(QVulkanWindow *w) : m_window(w) {}

    void initResources() override {

    }

    void initSwapChainResources() override {

    }

    void releaseSwapChainResources() override {

    }

    void releaseResources() override {

    }

    void startNextFrame() override {
        VkCommandBuffer cmdBuf = m_window->currentCommandBuffer();
        VkRenderPassBeginInfo rpbi = m_window->defaultRenderPass();
    }
private: 
    QVulkanWindow *m_window;
};