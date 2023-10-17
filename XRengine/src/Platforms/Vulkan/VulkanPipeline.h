#pragma once

#include <vulkan\vulkan.h>
#include "VulkanRenderPass.h"

namespace XRE {

	class VulkanPipeline {
	public:
		VulkanPipeline() {

		}
		~VulkanPipeline() {

		}
		void Init();
		void CleanUp(VkDevice device);

		void SetPass(XRef<VulkanRenderPass> pass) {
			RenderPass = pass;
		}
	

		VkPipeline pipeline;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;

		XRef<VulkanRenderPass> RenderPass;
	};
}