#include "pch.h"
#include "Light.h"
#include "Renderer.h"
#include "Platforms\OpenGL\OpenGLShader.h"


namespace XRE {
	Light::Light()
		:m_DirectionalLight(std::make_shared<DirectionalLight>(glm::vec3(-0.2f, -1.0f, 0.3f)))
	{
		
	}
	void Light::Draw(Ref<Shader> shader)
	{
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {
			auto& openglshader = std::dynamic_pointer_cast<OpenGLShader>(shader);

			openglshader->Bind();
			openglshader->setFloat3("d_light.direction", m_DirectionalLight->m_Direction);
			openglshader->setFloat3("d_light.color", m_DirectionalLight->m_Color);

			int index = 0;
			for (auto p : m_PointLights) {

				openglshader->setFloat3("p_light["+ std::to_string(index) + "].position" , p->m_Position);
				openglshader->setFloat3("p_light[" + std::to_string(index) + "].color", p->m_Color);
				openglshader->setFloat("p_light[" + std::to_string(index) + "].intensity", p->m_Intensity);
				openglshader->setInt("p_light[" + std::to_string(index) + "].enable", 1);
				index++;

			}
		}
	}
	void Light::SetDirLight(Ref<DirectionalLight> d)
	{
		m_DirectionalLight = d;
	}
	void Light::ClearPLights()
	{
		m_PointLights.clear();
	}
	void Light::AddPLight(Ref<PointLight> p)
	{
		m_PointLights.push_back(p);

	}
	
}