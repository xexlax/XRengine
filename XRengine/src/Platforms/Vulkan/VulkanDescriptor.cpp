#include "VkContext.h"
#include "VulkanDescriptor.h"



XRE::VulkanDescriptorPool::VulkanDescriptorPool(uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes)
{
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descriptorPoolInfo.pPoolSizes = poolSizes.data();
	descriptorPoolInfo.maxSets = maxSets;
	//descriptorPoolInfo.flags = poolFlags;

	if (vkCreateDescriptorPool(VkContext::GetDevice(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

}

XRE::VulkanDescriptorPool::~VulkanDescriptorPool()
{
	vkDestroyDescriptorPool(VkContext::GetDevice(), descriptorPool, nullptr);
}

bool XRE::VulkanDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
{
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	allocInfo.descriptorSetCount = 1;
	if (vkAllocateDescriptorSets(VkContext::GetDevice(), &allocInfo, &descriptor) != VK_SUCCESS) {
		return false;
	}
	return true;
	
}

void XRE::VulkanDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
{
	vkFreeDescriptorSets(
		VkContext::GetDevice(),
		descriptorPool,
		static_cast<uint32_t>(descriptors.size()),
		descriptors.data());

}

void XRE::VulkanDescriptorPool::resetPool()
{
	vkResetDescriptorPool(VkContext::GetDevice(), descriptorPool, 0);

}


XRE::VulkanDescriptorWriter::VulkanDescriptorWriter(XRef<VulkanPipeline> pipeline)
{
    createDescriptorSets(pipeline);
    
}

void XRE::VulkanDescriptorWriter::createDescriptorSets(XRef<VulkanPipeline> pipeline) {


    std::vector<VkDescriptorSetLayout> layouts(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT, pipeline->descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = VkContext::GetInstance()->descriptorPool->descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();



    descriptorSets.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(VkContext::GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    writes =  vector<VkWriteDescriptorSet>(2);
   

}

void XRE::VulkanDescriptorWriter::writeBuffer(XRef<VulkanUniformBuffer> buffer)
{
  

        writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[0].dstBinding = 0;
        writes[0].dstArrayElement = 0;
        writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[0].descriptorCount = 1;
        writes[0].pBufferInfo = buffer->GetBufferInfo();

    
}

void XRE::VulkanDescriptorWriter::writeImage(XRef<VulkanImage> image, uint32_t offset)
{
   

        

        writes[offset].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[offset].dstBinding = offset;
        writes[offset].dstArrayElement = 0;
        writes[offset].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[offset].descriptorCount = 1;
        writes[offset].pImageInfo = image->GetImageInfo();

        
}

void XRE::VulkanDescriptorWriter::overwrite(int i)
{
    for (auto &x : writes) {
        x.dstSet = descriptorSets[i];
    }
    vkUpdateDescriptorSets(VkContext::GetDevice(), writes.size(), writes.data(), 0, nullptr);

}
