#include "Engine/Renderer/Vulkan/VulkanRenderer.hpp"

void VulkanRenderer::initResources()
{
    m_devFuncs = m_window->vulkanInstance()->deviceFunctions(m_window->device());
}

void VulkanRenderer::initSwapChainResources() {

}

void VulkanRenderer::releaseSwapChainResources() {

}

void VulkanRenderer::releaseResources() {

}

void VulkanRenderer::startNextFrame()
{
    VkClearColorValue clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
    VkClearDepthStencilValue clearDepth = {1.0f, 0};
    VkClearValue clearValues[2];

    memset(clearValues, 0, sizeof(clearColor));

    clearValues[0].color = clearColor;
    clearValues[1].depthStencil = clearDepth;

    VkRenderPassBeginInfo rpBeginInfo;

    memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = m_window->defaultRenderPass();
    rpBeginInfo.framebuffer = m_window->currentFramebuffer();

    const QSize sz = m_window->swapChainImageSize();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    rpBeginInfo.clearValueCount = 2;
    rpBeginInfo.pClearValues = clearValues;

    VkCommandBuffer cmdBuf = m_window->currentCommandBuffer();
    m_devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    m_devFuncs->vkCmdEndRenderPass(cmdBuf);

    m_window->frameReady();
    m_window->requestUpdate();
}