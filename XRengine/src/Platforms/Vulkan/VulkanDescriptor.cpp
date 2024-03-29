#include "VkContext.h"
#include "VulkanDescriptor.h"

#include <xutility>

XRE::VulkanDescriptorPool::VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolSize* pool_sizes)
{
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.poolSizeCount = 11;
	descriptorPoolInfo.pPoolSizes = pool_sizes;
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

    writes =  vector<VkWriteDescriptorSet>();
    

}

void XRE::VulkanDescriptorWriter::writeBuffer(XRef<VulkanUniformBuffer> buffer)
{
  
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding = writes.size();
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.descriptorCount = 1;
    write.pBufferInfo = buffer->GetBufferInfo();

    writes.push_back(write);
    
}

void XRE::VulkanDescriptorWriter::writeImage(XRef<VulkanImage> image)
{
  
    
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding = writes.size();
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    if(image)
        write.pImageInfo = image->GetImageInfo();
    writes.push_back(write);
        
}

void XRE::VulkanDescriptorWriter::overwrite(int i)
{
    for (auto &x : writes) {
        x.dstSet = descriptorSets[i];
    }
    vkUpdateDescriptorSets(VkContext::GetDevice(), writes.size(), writes.data(), 0, nullptr);
    writes.clear();
}
