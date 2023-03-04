#include "pch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platforms\OpenGL\OpenGLFrameBuffer.h"
namespace XRE {
	
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec);
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}