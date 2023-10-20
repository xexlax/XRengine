
#include "pch.h"
#include <glad\glad.h>
#include "Shader.h"
#include <glm\gtc\type_ptr.hpp>
#include "xre\Renderer\Renderer.h"
#include "Platforms\OpenGL\OpenGLShader.h"
#include "Platforms\Vulkan\VulkanShader.h"
namespace XRE {
	
    XRef<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanShader>(name, vertexSrc, fragmentSrc);
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }

	XRef<Shader> Shader::Create(const std::string& src)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  
		{
			XRef<Shader> s = std::make_shared<OpenGLShader>(src);
			s->m_Path = src;
			return s;
		}

		case RendererAPI::API::Vulkan:
		{
			XRef<Shader> s = std::make_shared<VulkanShader>(src);
			s->m_Path = src;
			return s;
		}
			
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	


}