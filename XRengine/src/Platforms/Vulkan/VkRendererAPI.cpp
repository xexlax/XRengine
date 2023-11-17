#include "pch.h"
#include "VkRendererAPI.h"
#include "VkContext.h"


namespace XRE {

	


	void VkRendererAPI::SetClearColor(const glm::vec4& color)
	{
	}

	void VkRendererAPI::Clear()
	{
	}

	void VkRendererAPI::Init()
	{
	}

	void VkRendererAPI::CullFace(bool b)
	{
	}

	void VkRendererAPI::SetDepthTest(bool b)
	{
	}

	void VkRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{

		int currentFrame = VkContext::GetInstance()->swapChain->currentFrame;
		VkCommandBuffer commandBuffer = VkContext::GetInstance()->commandBuffers[currentFrame];
		std::dynamic_pointer_cast<VulkanVertexArray>(vertexArray)->Bind(commandBuffer);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
			VkContext::GetInstance()->pipeline->pipelineLayout, 0, 1, 
			&VkContext::GetInstance()->descriptorSets[currentFrame], 0, nullptr);
		std::dynamic_pointer_cast<VulkanVertexArray>(vertexArray)->Draw(commandBuffer);
	}

	void VkRendererAPI::DrawIndexedLine(const std::shared_ptr<VertexArray>& vertexArray)
	{
	}

	void VkRendererAPI::DrawIndexedLineLoop(const std::shared_ptr<VertexArray>& vertexArray)
	{
	}

	void VkRendererAPI::DrawSkyIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t cubemapTexture)
	{
	}

	void VkRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		VkContext::GetInstance()->swapChain->swapChainExtent.width = width;
		VkContext::GetInstance()->swapChain->swapChainExtent.height = height;
		VkContext::GetInstance()->swapChain->recreateSwapChain();
	}

}