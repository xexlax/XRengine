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
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanRHI.h"
#include "VulkanUtils.h"
#include "VulkanDescriptor.h"
#include "xre\Renderer\Renderer.h"
#include "VulkanStructs.h"
#define XRE_VK_INSTANCE VkContext::GetInstance()

namespace XRE {
	class VkContext: public GraphicsContext
	{
	public:
		VkContext(GLFWwindow* windowHandle);

        
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
        static VkContext* ContextInstance;

	public:
        static VkContext* GetInstance() { return ContextInstance; }

        static VkDevice GetDevice() { return GetInstance()->device; }
        
        static GLFWwindow* GetWindow() { return GetInstance()->m_WindowHandle; }

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        XRef<VulkanSwapChain> swapChain;
        XRef<ModelPipeline> modelPipeline;
        XRef<SkyBoxPipeline> skyboxPipeline;

        VkCommandPool commandPool;
        
        std::vector <XRef<VulkanUniformBuffer>> GlobalUBOs;
        
        XRef<VulkanDescriptorPool> descriptorPool;
        std::vector<VkCommandBuffer> commandBuffers;

        //current
        uint32_t imageIndex;

        bool framebufferResized = false;

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            auto app = reinterpret_cast<VkContext*>(glfwGetWindowUserPointer(window));
            app->framebufferResized = true;
        }

        virtual void Init() override
        {

            ContextInstance = this;
            createInstance();
            setupDebugMessenger();
            createSurface();
            pickPhysicalDevice();
            createLogicalDevice();


            swapChain = XMakeRef<VulkanSwapChain>(physicalDevice, device);
            modelPipeline = XMakeRef<ModelPipeline>(swapChain->renderPass);
            createCommandPool();
            swapChain->createDepthResources();
            swapChain->createFramebuffers();
            createCommandBuffers();
            swapChain->createSyncObjects();
            //´´½¨ÃèÊö·û³Ø
            createDescriptorPool();
            modelPipeline->CreateUBOs();
           

        }




        void cleanup() {
            swapChain->CleanUp(device);

            modelPipeline->CleanUp(device);

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

        

        void createDescriptorPool();

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

        

        void beginPass(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            swapChain->BindRenderPass(commandBuffer, imageIndex);
            modelPipeline->Bind(commandBuffer);


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

        }


        void endPass(VkCommandBuffer commandBuffer){
            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }


        void beginFrame(Renderer::SceneData* scenedata) {


            imageIndex = swapChain->acquireNextImage();
            uint32_t currentFrame = swapChain->currentFrame;


            UniformBufferObject ubo{};
        
            ubo.view = scenedata->ViewMatrix;
            ubo.proj = scenedata->ProjectionMatrix;
            ubo.proj[1][1] *= -1;

            LightingUBO lubo{};

            auto dlight = scenedata->lightSystem->getDirLight();
            lubo.d.dir =  glm::vec4(dlight.m_Direction,1.0f);
            lubo.d.color = dlight.m_Color;
            lubo.d.intensity = dlight.m_Intensity;

            lubo.plightCount = scenedata->lightSystem->getPLightCount();
            if (lubo.plightCount > 4) lubo.plightCount = 4;

            for (int i = 0;i < lubo.plightCount;i++) {
                auto plight = scenedata->lightSystem->getPointLight(i);
                lubo.p[i].pos = glm::vec4(plight.m_Position, 0.0f);
                lubo.p[i].color = plight.m_Color;
                lubo.p[i].intensity = plight.m_Intensity;
            }

            modelPipeline->GlobalUBOs[currentFrame]->WriteToBuffer(&ubo);
            modelPipeline->LightingUBOs[currentFrame]->WriteToBuffer(&lubo);
            

            vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
            beginPass(commandBuffers[currentFrame], imageIndex);

            
        }
           
            
        void endFrame(){
            uint32_t currentFrame = swapChain->currentFrame;
            endPass(commandBuffers[currentFrame]);
            
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

        void PushConstant(void* data, uint32_t size) {
            vkCmdPushConstants(
                commandBuffers[swapChain->currentFrame],
                modelPipeline->pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                size,
                data);
        }


	};
}
