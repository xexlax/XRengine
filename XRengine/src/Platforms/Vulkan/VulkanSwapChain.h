#pragma once
#include <vulkan/vulkan.h>
#include "VulkanImage.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRenderPass.h"

namespace XRE {

    
    
	class VulkanSwapChain {

    public:
        const static int MAX_FRAMES_IN_FLIGHT = 2;


        VulkanSwapChain(const VkPhysicalDevice& physicalDevice, const VkDevice& device);
        void Init(const VkPhysicalDevice& physicalDevice, const VkDevice& device);
        void CleanUp(const VkDevice& device);

        VkSwapchainKHR swapChain;

       
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;

        // FrameBuffer
        
        std::vector<VkFramebuffer> swapChainFramebuffers;
        std::vector < XRef<VulkanImage>> ColorImage;
        XRef<VulkanImage> DepthImage;


        XRef<VulkanRenderPass> renderPass;
        XRef<VulkanRenderPass> UIPass;
        uint32_t currentFrame = 0;

        void NextFrame() {
            currentFrame = (currentFrame + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
        }

        void createFramebuffers();

        void recreateSwapChain();

        void createImageViews();

        void createRenderPass(VkFormat swapChainImageFormat);

        void createDepthResources();

        void createSyncObjects();

        void BindRenderPass(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void BindUIPass(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        uint32_t acquireNextImage();

        VkResult submitCommandBuffer(const std::vector<VkCommandBuffer>& buffers, uint32_t* imageIndex);

    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;


        

        

	};
}