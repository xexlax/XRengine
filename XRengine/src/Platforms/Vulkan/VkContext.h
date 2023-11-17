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
#include "VulkanBuffers.h"
#include "VulkanDescriptor.h"
#include "xre\Renderer\Renderer.h"
#include "VulkanTexture.h"
#define XRE_VK_INSTANCE VkContext::GetInstance()



const std::string VERT_PATH = "shaders/default_vert.spv";
const std::string FRAG_PATH = "shaders/default_frag.spv";
const std::string TEXTURE_PATH = "textures/viking_room.png";


struct UniformBufferObject {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

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
        XRef<VulkanPipeline> pipeline;

        VkCommandPool commandPool;
        
        std::vector <XRef<VulkanUniformBuffer>> uniformBuffers;
        
        XRef<VulkanDescriptorPool> descriptorPool;
        std::vector<VkCommandBuffer> commandBuffers;

        XRef<VulkanDescriptorWriter> globalDW;

        XRef<VulkanTexture2D> tex;

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
            pipeline = XMakeRef<VulkanPipeline>(VERT_PATH, FRAG_PATH, swapChain->renderPass);
            createCommandPool();
            swapChain->createDepthResources();
            swapChain->createFramebuffers();
            createCommandBuffers();
            swapChain->createSyncObjects();
            //创建描述符池
            createDescriptorPool();
            //根据UBO结构 创建UniformBuffer
            createUniformBuffers();

            //对每个VAO，单独创建ds
            //createDescriptorSets();
            tex = XMakeRef<VulkanTexture2D>(TEXTURE_PATH);
            globalDW = XMakeRef<VulkanDescriptorWriter>();
            for (int i = 0;i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;i++) {
                globalDW->writeBuffer(VkContext::GetInstance()->uniformBuffers[i]);
                globalDW->writeImage(tex->m_Image, 1);
                globalDW->overwrite(i);
            }


        }




        void cleanup() {
            swapChain->CleanUp(device);

            pipeline->CleanUp(device);

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

            uniformBuffers[currentFrame]->WriteToBuffer(&ubo);

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
                pipeline->pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                size,
                data);
        }


	};
}
