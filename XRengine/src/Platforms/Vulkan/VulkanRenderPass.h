#pragma once

#include <vulkan/vulkan.h>

namespace XRE {
	class VulkanRenderPass {

	public:
		VulkanRenderPass(VkDevice device) {
			Init(device);
		}

		VkRenderPass renderPass;

		void Init(VkDevice device);
		void CleanUp(VkDevice device);
	};


}