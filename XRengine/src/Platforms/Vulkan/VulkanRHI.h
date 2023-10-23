#pragma once
#include <vulkan\vulkan.h>
#include "vk_mem_alloc.h"
#include <GLFW\glfw3.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace XRE {
   

    class VulkanRHI
    {
    public:

        static std::vector<const char*> getRequiredExtensions();
        
        static bool isDeviceSuitable(VkPhysicalDevice device);
		
        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        
        static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        
        static VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        
        static VkFormat findDepthFormat();
        
        static bool hasStencilComponent(VkFormat format);

        static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

        static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        static VkCommandBuffer beginSingleTimeCommands();

        static void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    };
}