#include "pch.h"
#include "RenderCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"
#include "Platforms/Vulkan/VkRendererAPI.h"

namespace XRE {

#ifdef XRE_RENDERER_OPENGL
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
#endif // XRE_RENDERER_OPENGL

#ifdef XRE_RENDERER_VULKAN
	RendererAPI* RenderCommand::s_RendererAPI = new VkRendererAPI;
#endif // XRE_RENDERER_VULKAN


	

	

}