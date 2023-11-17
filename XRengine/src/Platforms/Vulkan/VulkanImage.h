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

		VkDescriptorImageInfo* GetImageInfo() {

			imageDescInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageDescInfo.imageView = ImageView;
			imageDescInfo.sampler = Sampler;
			return &imageDescInfo;
		}

		VkDescriptorImageInfo imageDescInfo{};
		

		void Create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

		void CleanUp();

        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
	};



	
}