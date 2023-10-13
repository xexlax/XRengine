#include "pch.h"
#include "RendererAPI.h"



namespace XRE{

#ifdef XRE_RENDERER_OPENGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif // XRE_RENDERER_OPENGL

#ifdef XRE_RENDERER_VULKAN
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#endif // XRE_RENDERER_VULKAN


	
	

}