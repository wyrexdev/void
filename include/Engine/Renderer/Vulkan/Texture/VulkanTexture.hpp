#ifndef VULKAN_TEXTURE_HPP
#define VULKAN_TEXTURE_HPP

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <string>

class VulkanDevice;
class VulkanCommandPool;

class VulkanTexture {
public:
    VulkanTexture(VulkanDevice* device, VulkanCommandPool* commandPool);
    ~VulkanTexture();

    bool createFromData(const void* data, uint32_t width, uint32_t height, VkFormat format, VkFilter filter = VK_FILTER_LINEAR);
    bool createFromFile(const std::string& filename, VkFilter filter = VK_FILTER_LINEAR);

    void destroy();

    VkImageView getImageView() const { return m_imageView; };

private:
    VulkanDevice* m_device;
    VulkanCommandPool* m_commandPool;

    VkImage m_image;
    VkDeviceMemory m_imageMemory;
    VkImageView m_imageView;
};

#endif