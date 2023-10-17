#include "pch.h"
#include "Texture.h"

#include "xre\Renderer\Renderer.h"

#ifdef XRE_RENDERER_OPENGL

	#include "Platforms/OpenGL/OpenGLTexture.h"

#else

	#include "Platforms\Vulkan\VulkanTexture.h"

#endif // XRE_RENDERER_OPENGL
namespace XRE {

	XRef<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

#ifdef XRE_RENDERER_OPENGL
		case RendererAPI::API::OpenGL:  
			
			XRef<Texture2D> s= std::make_shared<OpenGLTexture2D>(path);
			s->m_Path = path;
			return s;
#else

		case RendererAPI::API::Vulkan:

			XRef<Texture2D> s = std::make_shared<VulkanTexture2D>(path);
			s->m_Path = path;
			return s;
#endif

		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	

}