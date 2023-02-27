#include "pch.h"
#include "Light.h"
#include "Renderer.h"
#include "Platforms\OpenGL\OpenGLShader.h"

#include <xre\Utils\TransformUnpacking.h>


namespace XRE {
	Light::Light()
		:m_DirectionalLight(glm::vec3(-0.2f, -1.0f, 0.3f)),m_PointLights(2)
	{
		
	}
	void Light::Draw(Ref<Shader> shader)
	{
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {
			auto& openglshader = std::dynamic_pointer_cast<OpenGLShader>(shader);

			openglshader->Bind();
			openglshader->SetFloat3("d_light.direction", m_DirectionalLight.m_Direction);
			openglshader->SetFloat("d_light.intensity", m_DirectionalLight.m_Intensity);
			openglshader->SetFloat3("d_light.color", m_DirectionalLight.m_Color);

			int index = 0;
			for (const PointLightComponent& p : m_PointLights) {
				
				openglshader->SetFloat3("p_light["+ std::to_string(index) + "].position" , p.m_Position);
				openglshader->SetFloat3("p_light[" + std::to_string(index) + "].color", p.m_Color);
				openglshader->SetFloat("p_light[" + std::to_string(index) + "].intensity", p.m_Intensity);
				openglshader->SetInt("p_light[" + std::to_string(index) + "].enable", 1);
				index++;

			}
		}
	}
	
	void Light::SetDirLight(GameObject &go)
	{
		m_DirectionalLight  = go.GetComponent<DirectionalLightComponent>();
		
	}
	void Light::ClearPLights()
	{
		m_PointLights.clear();
	}

	void Light::SetPLight(GameObject& go,int i)
	{
		//TransformComponent t = go.GetComponent<TransformComponent>();
		PointLightComponent l = go.GetComponent<PointLightComponent>();
		m_PointLights[i]=l;
	}

	void Light::RemovePLight(GameObject& go)
	{
	}
	
}