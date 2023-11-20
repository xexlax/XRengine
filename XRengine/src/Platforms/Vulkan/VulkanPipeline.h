#pragma once

#include <vulkan\vulkan.h>
#include "VulkanRenderPass.h"
#include "VulkanShader.h"



namespace XRE {

	const std::string VERT_PATH = "shaders/default_vert.spv";
	const std::string FRAG_PATH = "shaders/default_frag.spv";

	struct PipelineConfigInfo {
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VulkanPipeline {
	public:
		VulkanPipeline(){}
		
		~VulkanPipeline() {

		}
		void Init();
		void CleanUp(VkDevice device);

		void Bind(VkCommandBuffer commandBuffer);

		

		VkPipeline pipeline;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;

		
	};

	class ModelPipeline :public VulkanPipeline {
	public:
		ModelPipeline(
			XRef<VulkanRenderPass> renderPass);
	};

	class SkyBoxPipeline :public VulkanPipeline {
	public:
		SkyBoxPipeline(
			XRef<VulkanRenderPass> renderPass);
		
	};
}