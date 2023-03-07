#include "pch.h"
#include "VertexArrays.h"

#include "xre\Renderer\Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace XRE {

	XRef<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}