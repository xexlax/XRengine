#include "pch.h"
#include "VkRendererAPI.h"

#include <glad/glad.h>

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
	}

}