#pragma once
#include <vulkan/vulkan.h>
#include "VulkanImage.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRenderPass.h"

namespace XRE {

    
    
	class VulkanSwapChain {

    public:

        VulkanSwapChain(const VkPhysicalDevice& physicalDevice, const VkDevice& device);
        void Init(const VkPhysicalDevice& physicalDevice, const VkDevice& device);
        void CleanUp(const VkDevice& device);

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        XRef<VulkanImage> DepthImage;
        XRef<VulkanRenderPass> renderPass;

        void createFramebuffers();

        void recreateSwapChain();

        void createImageViews();

        void createRenderPass(VkFormat swapChainImageFormat);

        void createDepthResources();

        void BindRenderPass(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        

	};
}