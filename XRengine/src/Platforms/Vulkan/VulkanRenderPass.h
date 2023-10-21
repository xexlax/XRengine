#pragma once

#include <vulkan/vulkan.h>

namespace XRE {
	class VulkanRenderPass {

	public:
		VulkanRenderPass(VkDevice device, VkFormat swapChainImageFormat) {
			Init(device, swapChainImageFormat);
		}

		VkRenderPass renderPass;

		void Init(VkDevice device, VkFormat swapChainImageFormat);
		void CleanUp(VkDevice device);
	};


}