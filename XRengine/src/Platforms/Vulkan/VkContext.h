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

        static VkCommandBuffer GetCommandBuffer() { return GetInstance()->commandBuffers[GetInstance()->swapChain->currentFrame]; }

        static VkCommandBuffer GetImGuiCommandBuffer() { return GetInstance()->imguicommandBuffers[GetInstance()->swapChain->currentFrame]; }

        
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
        std::vector<VkCommandBuffer> commandBuffers,imguicommandBuffers;

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

        void createCommandBuffers();

        

        void beginPass(VkCommandBuffer commandBuffer, uint32_t imageIndex);


        void endPass(VkCommandBuffer commandBuffer);

        void beginScene(Renderer::SceneData* scenedata);

        void endScene();

        void beginFrame();
            
        void endFrame();

        void PushConstant(void* data, uint32_t size);



	};
}
