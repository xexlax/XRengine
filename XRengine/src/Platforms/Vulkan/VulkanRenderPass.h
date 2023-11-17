#pragma once

#include "xre\Renderer\Resources\RenderPass.h"
#include <vulkan/vulkan.h>

namespace XRE {
	class VulkanRenderPass:public RenderPass {

	public:
		VulkanRenderPass(VkDevice device, VkFormat swapChainImageFormat) {
			Init(device, swapChainImageFormat);
		}

		VkRenderPass renderPass;

		void Init(VkDevice device, VkFormat swapChainImageFormat);
		void CleanUp(VkDevice device);

		virtual void Forward()	override;
		
	};


}