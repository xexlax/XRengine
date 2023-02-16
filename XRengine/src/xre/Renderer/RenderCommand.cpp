#include "pch.h"
#include "RenderCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace XRE {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}