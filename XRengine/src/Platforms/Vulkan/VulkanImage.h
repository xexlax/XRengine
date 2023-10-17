#pragma once
#include <vulkan\vulkan.h>
namespace XRE {
	class VulkanImage
	{
	public:
		VkImage Image;
		VkDeviceMemory ImageMemory;
		VkImageView ImageView;
		VkSampler Sampler;

		void Create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

		void CleanUp();

        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
	};



	
}