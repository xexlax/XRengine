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

XRE::VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool)
	: setLayout{ setLayout }, pool{ pool }
{

}

void XRE::VulkanDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
{
	assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

	auto& bindingDescription = setLayout.bindings[binding];

	assert(
		bindingDescription.descriptorCount == 1 &&
		"Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = binding;
	write.pBufferInfo = bufferInfo;
	write.descriptorCount = 1;

	writes.push_back(write);
}

void XRE::VulkanDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
{
	assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

	auto& bindingDescription = setLayout.bindings[binding];

	assert(
		bindingDescription.descriptorCount == 1 &&
		"Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = binding;
	write.pImageInfo = imageInfo;
	write.descriptorCount = 1;

	writes.push_back(write);
	
}

bool XRE::VulkanDescriptorWriter::build(VkDescriptorSet& set)
{
	bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
	if (!success) {
		return false;
	}
	overwrite(set);
	return true;
}

void XRE::VulkanDescriptorWriter::overwrite(VkDescriptorSet& set)
{
	for (auto& write : writes) {
		write.dstSet = set;
	}
	vkUpdateDescriptorSets(VkContext::GetDevice(), writes.size(), writes.data(), 0, nullptr);

}




XRE::VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
	:bindings{bindings}
{




	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
	for (auto kv : bindings) {
		setLayoutBindings.push_back(kv.second);
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
	descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
	descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

	if (vkCreateDescriptorSetLayout(
		VkContext::GetDevice(),
		&descriptorSetLayoutInfo,
		nullptr,
		&descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}



XRE::VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(VkContext::GetDevice(), descriptorSetLayout, nullptr);
}

XRE::VulkanDescriptorSetLayout::Builder& XRE::VulkanDescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
{
	assert(bindings.count(binding) == 0 && "Binding already in use");
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = binding;
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.descriptorCount = count;
	layoutBinding.stageFlags = stageFlags;
	bindings[binding] = layoutBinding;
	return *this;
}

std::unique_ptr<XRE::VulkanDescriptorSetLayout> XRE::VulkanDescriptorSetLayout::Builder::build() const
{
	return std::make_unique<VulkanDescriptorSetLayout>( bindings);
}
