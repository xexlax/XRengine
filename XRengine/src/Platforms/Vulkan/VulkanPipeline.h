#pragma once

#include <vulkan\vulkan.h>
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "VulkanBuffers.h"



namespace XRE {

	

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

		virtual void CreateUBOs()=0;

		VkPipeline pipeline;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;

		
	};

	class ModelPipeline :public VulkanPipeline {
	public:
		ModelPipeline(
			XRef<VulkanRenderPass> renderPass);

		std::vector <XRef<VulkanUniformBuffer>> GlobalUBOs;
		std::vector <XRef<VulkanUniformBuffer>> LightingUBOs;
		

		void CreateUBOs() override;
	};


	class DeferredPipeline :public VulkanPipeline {
	public:
		DeferredPipeline(
			XRef<VulkanRenderPass> renderPass);

		std::vector <XRef<VulkanUniformBuffer>> GlobalUBOs;
		std::vector <XRef<VulkanUniformBuffer>> LightingUBOs;


		void CreateUBOs() override;
	};


	class SkyBoxPipeline :public VulkanPipeline {
	public:
		SkyBoxPipeline(
			XRef<VulkanRenderPass> renderPass);

		std::vector <XRef<VulkanUniformBuffer>> GlobalUBOs;


		void CreateUBOs() override;
		
	};
}