#pragma once
#include <vulkan\vulkan.h>

namespace XRE {
	class VulkanImage
	{

	public:

		~VulkanImage() {
			CleanUp();
		}

		VkImage Image;
		VkDeviceMemory ImageMemory;
		VkImageView ImageView;
		VkSampler Sampler;
		VkImageLayout Layout;
		VkFormat Format;
		

		VkDescriptorImageInfo* GetImageInfo() {

			imageDescInfo.imageLayout = Layout;
			imageDescInfo.imageView = ImageView;
			imageDescInfo.sampler = Sampler;
			return &imageDescInfo;
		}

		VkDescriptorImageInfo imageDescInfo{};
		

		void Create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, 
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		void CleanUp();

        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);

		void* GetDescriptorSet();

		void createAttachment(VkFormat format,	VkImageUsageFlagBits usage ,int32_t width, int32_t height);

	private:
		bool registered = false;

		VkDescriptorSet ds;
		
	};



	
}