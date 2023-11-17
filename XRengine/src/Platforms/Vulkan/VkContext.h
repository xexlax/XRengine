#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include "xre\Renderer\GraphicContext.h"
#include <glm\glm.hpp>

#include <pch.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


#include <vulkan\vulkan.h>

#include "VulkanSwapChain.h"
#include "VulkanTexture.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "xre\Resource\Model.h"
#include "VulkanRHI.h"
#include "VulkanUtils.h"
#include "VulkanVertexArray.h"
#include "VulkanDescriptor.h"
#include "xre\Renderer\Camera\EditorCamera.h"
#include "VulkanBuffers.h"
#include "xre\Renderer\Renderer.h"
#define XRE_VK_INSTANCE VkContext::GetInstance()


const std::string MODEL_PATH = "models/viking_room.obj";
const std::string TEXTURE_PATH = "textures/viking_room.png";
const std::string VERT_PATH = "shaders/default_vert.spv";
const std::string FRAG_PATH = "shaders/default_frag.spv";










struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

namespace XRE {
	class VkContext: public GraphicsContext
	{
	public:
		VkContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
        static VkContext* ContextInstance;

	public:
        static VkContext* GetInstance() {
            return ContextInstance;
        }

        static VkDevice GetDevice() {
            return GetInstance()->device;
        }
        
        static GLFWwindow* GetWindow() {
            return GetInstance()->m_WindowHandle;
        }

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        XRef<VulkanSwapChain> swapChain;
        XRef<VulkanPipeline> pipeline;

        VkCommandPool commandPool;

        XRef<VulkanTexture2D> Texture;
        XRef<Model> VikingModel;
        
        std::vector <XRef<VulkanUniformBuffer>> uniformBuffers;
        
        XRef<VulkanDescriptorPool> descriptorPool;
        XRef<VulkanDescriptorWriter> descriptorWriter;
        //VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkCommandBuffer> commandBuffers;

        bool framebufferResized = false;

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            auto app = reinterpret_cast<VkContext*>(glfwGetWindowUserPointer(window));
            app->framebufferResized = true;
        }


        void LoadResource() {
            
            Texture = XMakeRef<VulkanTexture2D>(TEXTURE_PATH);
            
            VikingModel = XMakeRef<Model>(MODEL_PATH);

            //创建描述符集（类似Uniform）
            createDescriptorSets();
        }


        void cleanup() {
            swapChain->CleanUp(device);

            pipeline->CleanUp(device);

            Texture->m_Image->CleanUp();

            //destroy descriptor layout
            
            vkDestroyCommandPool(device, commandPool, nullptr);

            vkDestroyDevice(device, nullptr);

            if (enableValidationLayers) {
                VulkanDebugUtils::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
            }

            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);
            //glfwTerminate();
        }

        void createInstance();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        void setupDebugMessenger();

        void createSurface();

        void pickPhysicalDevice();

        void createLogicalDevice();

        void createCommandPool();


        void createUniformBuffers() {
            VkDeviceSize bufferSize = sizeof(UniformBufferObject);
            uniformBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
            for (size_t i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
                uniformBuffers[i] = XMakeRef<VulkanUniformBuffer>(GetDevice(), bufferSize);
            }
        }

        void createDescriptorPool() {
            std::vector<VkDescriptorPoolSize> poolSizes(2);
            poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSizes[0].descriptorCount = static_cast<uint32_t>(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

            poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSizes[1].descriptorCount = static_cast<uint32_t>(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

            descriptorPool = XMakeRef<VulkanDescriptorPool>(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT, poolSizes);
        }

        void createDescriptorSets() {


            std::vector<VkDescriptorSetLayout> layouts(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT, pipeline->descriptorSetLayout);
            
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool =  descriptorPool->descriptorPool;
            allocInfo.descriptorSetCount = static_cast<uint32_t>(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
            allocInfo.pSetLayouts = layouts.data();

           

            descriptorSets.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
            if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate descriptor sets!");
            }

            for (size_t i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers[i]->GetBuffer();
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(UniformBufferObject);

                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = Texture->m_Image->ImageView;
                imageInfo.sampler = Texture->m_Image->Sampler;

                std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = descriptorSets[i];
                descriptorWrites[0].dstBinding = 0;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &bufferInfo;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = descriptorSets[i];
                descriptorWrites[1].dstBinding = 1;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pImageInfo = &imageInfo;

                vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }
        }
        
        void createCommandBuffers() {
            commandBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = commandPool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

            if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate command buffers!");
            }
        }

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            swapChain->BindRenderPass(commandBuffer, imageIndex);
            pipeline->Bind(commandBuffer);

            
            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)swapChain->swapChainExtent.width;
            viewport.height = (float)swapChain->swapChainExtent.height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = { 0, 0 };
            scissor.extent = swapChain->swapChainExtent;
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            //model->Bind(commandBuffer);

            XRef<VulkanVertexArray> vao = std::dynamic_pointer_cast<VulkanVertexArray>(VikingModel->m_Meshes[0].GetVAO());
            vao->Bind(commandBuffer);

            uint32_t currentFrame = swapChain->currentFrame;

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

            vao->Draw(commandBuffer);
            //model->Draw(commandBuffer);
            
            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }


        void drawFrame(Renderer::SceneData *scenedata) {
            

            uint32_t imageIndex = swapChain->acquireNextImage();
            uint32_t currentFrame = swapChain->currentFrame;


            UniformBufferObject ubo{};
            ubo.model = glm::mat4(1.0f);
            ubo.view = scenedata->ViewMatrix;
            ubo.proj = scenedata->ProjectionMatrix;
            ubo.proj[1][1] *= -1;

            uniformBuffers[currentFrame]->WriteToBuffer(&ubo);

            vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
            recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
            
            auto result = swapChain->submitCommandBuffer(commandBuffers, &imageIndex);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
                framebufferResized = false;
                swapChain->recreateSwapChain();
            }
            else if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to present swap chain image!");
            }

            currentFrame = (currentFrame + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
        }


	};

	
}
