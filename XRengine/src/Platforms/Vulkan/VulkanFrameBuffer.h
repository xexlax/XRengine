
#pragma once
#include "VulkanImage.h"
#include "xre\Renderer\FrameBuffer.h"

namespace XRE {

	
	class VulkanFramebuffer {
	public:

		class VulkanFramebufferBuilder {

			

		public:
			std::vector<std::pair<VkFormat,VkImageUsageFlagBits>> color_attachments;
			VkImageUsageFlagBits depth_usage;


		} builder;


		VulkanFramebuffer() {};

		
		void Recreate(int32_t w,int32_t h);

		void AddColor(VkFormat format, VkImageUsageFlagBits usage);
		void AddDepth(VkImageUsageFlagBits usage);
		

		std::vector<VkAttachmentDescription> GetAttachmentDescriptions();

	private:

		int32_t width, height;

		
		std::vector<XRef<VulkanImage>> ColorAttachments;
		XRef<VulkanImage> DepthAttachment;

		VkFramebuffer m_framebuffer;

		void CreateColorAttachment(VkFormat format, VkImageUsageFlagBits usage);
		void CreateDepthAttachment(VkImageUsageFlagBits usage);

	};
}