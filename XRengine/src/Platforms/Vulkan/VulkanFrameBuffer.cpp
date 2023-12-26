#include "D:/Study/bishe2023.1/dev/XRengine/XRengine/src/pch.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRHI.h"

void XRE::VulkanFramebuffer::CreateColorAttachment(VkFormat format, VkImageUsageFlagBits usage)
{
	auto attachment = XMakeRef<VulkanImage>();
	attachment->createAttachment(format, usage, width, height);
	ColorAttachments.push_back(attachment);
}

void XRE::VulkanFramebuffer::CreateDepthAttachment(VkImageUsageFlagBits usage)
{
	
	VkFormat format = VulkanRHI::findDepthFormat();
	
	DepthAttachment = XMakeRef<VulkanImage>();
	DepthAttachment->createAttachment(format, usage, width, height);


	
}

void XRE::VulkanFramebuffer::Recreate(int32_t w, int32_t h)
{
	ColorAttachments.clear();
	
	for (auto x : builder.color_attachments) {
		CreateColorAttachment(x.first, x.second);
	}
	CreateDepthAttachment(builder.depth_usage);
}

void XRE::VulkanFramebuffer::AddColor(VkFormat format, VkImageUsageFlagBits usage)
{
	builder.color_attachments.push_back({ format,usage });
	
}

void XRE::VulkanFramebuffer::AddDepth(VkImageUsageFlagBits usage)
{
	builder.depth_usage = usage;
}

std::vector<VkAttachmentDescription> XRE::VulkanFramebuffer::GetAttachmentDescriptions()
{

	std::vector<VkAttachmentDescription> attachmentDescs;
	attachmentDescs.resize(ColorAttachments.size() + 1);

	for (uint32_t i = 0; i <= ColorAttachments.size(); ++i)
	{
		attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		
		if (i == ColorAttachments.size())
		{
			attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			attachmentDescs[i].format = DepthAttachment->Format;
		}
		else
		{
			attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			attachmentDescs[i].format = ColorAttachments[i]->Format;
		}
	}

	return attachmentDescs;
}
