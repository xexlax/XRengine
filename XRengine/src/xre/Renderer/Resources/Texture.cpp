#include "pch.h"
#include "Texture.h"

#include "xre\Renderer\Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace XRE {

	XRef<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  
			
			XRef<Texture2D> s= std::make_shared<OpenGLTexture2D>(path);
			s->m_Path = path;
			return s;
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	

}