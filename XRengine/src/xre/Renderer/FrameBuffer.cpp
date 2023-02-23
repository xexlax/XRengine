#include "pch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platforms\OpenGL\OpenGLFrameBuffer.h"
namespace XRE {
	Framebuffer::Framebuffer(uint32_t width, uint32_t height):
		m_Height(height),m_Width(width)
	{
	}
	Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(width,height);
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}