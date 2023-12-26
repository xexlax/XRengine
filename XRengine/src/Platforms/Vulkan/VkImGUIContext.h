#pragma once

#include "vulkan\vulkan.h"
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"
#include "VulkanUtils.h"
#include "VkContext.h"

namespace XRE {
	class VkImGUIContext {

		
	public:
		static void init_imgui() {

			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};

			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000;
			pool_info.poolSizeCount = std::size(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;

			VkDescriptorPool imguiPool;
			;
			check_vk_result(vkCreateDescriptorPool(VkContext::GetDevice(), &pool_info, nullptr, &imguiPool));
			
			ImGui_ImplGlfw_InitForVulkan(VkContext::GetWindow(), true);
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = VkContext::GetInstance()->instance;
			init_info.PhysicalDevice = VkContext::GetInstance()->physicalDevice;
			init_info.Device = VkContext::GetInstance()->device;
			init_info.Queue = VkContext::GetInstance()->graphicsQueue;
			init_info.DescriptorPool = imguiPool;
			init_info.MinImageCount = 2;
			init_info.ImageCount = 2;
			ImGui_ImplVulkan_Init(&init_info, VkContext::GetInstance()->swapChain->UIPass->renderPass);
			
		}

		static void uploadFonts() {
			// Use any command queue

			VkCommandBuffer command_buffer = VkContext::GetInstance()->GetCommandBuffer();

			vkResetCommandPool(VkContext::GetInstance()->device, VkContext::GetInstance()->commandPool, 0);

			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkBeginCommandBuffer(command_buffer, &begin_info);


			ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

			VkSubmitInfo end_info = {};
			end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			end_info.commandBufferCount = 1;
			end_info.pCommandBuffers = &command_buffer;
			vkEndCommandBuffer(command_buffer);
			vkQueueSubmit(VkContext::GetInstance()->graphicsQueue, 1, &end_info, VK_NULL_HANDLE);


			vkDeviceWaitIdle(VkContext::GetInstance()->device);
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}
	};
}