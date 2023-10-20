#include "pch.h"
#include "Buffers.h"
#include "xre\Renderer\Renderer.h"
#include "Platforms\OpenGL\OpenGLBuffers.h"
#include "Platforms\Vulkan\VulkanBuffers.h"
#include "xre\Resource\Mesh.h"
namespace XRE {

	void VertexBuffer::Unbind() const
	{
	}
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		case RendererAPI::API::Vulkan:  return new VulkanVertexBuffer(vertices, size);
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	void IndexBuffer::Bind() const
	{
	}
	void IndexBuffer::Unbind() const
	{
	}
	uint32_t IndexBuffer::GetCount() const
	{
		return uint32_t();
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(indices, size);
		case RendererAPI::API::Vulkan: return new VulkanIndexBuffer(indices, size);
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
		
	}
}