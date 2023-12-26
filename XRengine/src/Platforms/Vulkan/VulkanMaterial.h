#pragma once
#include "xre\Resource\Material.h"
#include "VulkanDescriptor.h"

namespace XRE {
	class VulkanMaterial:public Material{

	public:

		VulkanMaterial(std::string path);

		void WriteDS();

		VkDescriptorSet* GetDescriptorSet(int offset) {
			return descriptorWriter->GetDescriptorSet(offset);
		}

		void Bind() override;

	private:
		bool setup = false;
		XRef<VulkanDescriptorWriter> descriptorWriter = nullptr;
		std::vector <XRef<VulkanUniformBuffer>> MaterialUBOs;
	};
}