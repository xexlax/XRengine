#include "pch.h"
#include "Light.h"
#include "Renderer.h"
#include "Platforms\OpenGL\OpenGLShader.h"




namespace XRE {
	Light::Light()
		:m_DirectionalLight(glm::vec3(-0.2f, -1.0f, 0.3f)),m_PointLights(2)
	{
		
	}
	void Light::Draw(Ref<Shader> shader)
	{
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {

			shader->Bind();
			shader->SetFloat3("d_light.direction", m_DirectionalLight.m_Direction);
			shader->SetFloat("d_light.intensity", m_DirectionalLight.m_Intensity);
			shader->SetFloat3("d_light.color", m_DirectionalLight.m_Color);

			int index = 0;
			for (auto p : m_PointLights) {
				
				shader->SetFloat3("p_light["+ std::to_string(index) + "].position" , p.m_Position);
				shader->SetFloat3("p_light[" + std::to_string(index) + "].color", p.m_Color);
				shader->SetFloat("p_light[" + std::to_string(index) + "].intensity", p.m_Intensity);
				shader->SetInt("p_light[" + std::to_string(index) + "].enable", 1);
				index++;

			}
			shader->SetInt("p_light_amount", index);
		}
	}
	
	void Light::SetDirLight(TransformComponent t, DirectionalLightComponent l)
	{		
		m_DirectionalLight = { glm::vec4(0,0,-1,0) * t.GetTransform() ,l.m_Color,l.m_Intensity };
		m_DirectionalLight.m_Rotation = t.m_Rotation;
	}
	void Light::ClearPLights()
	{
		m_PointLights.clear();
	}

	void Light::AddPLight(TransformComponent t, PointLightComponent l)
	{
		if(m_PointLights.size()<9)
		m_PointLights.push_back({ t.m_Translation,l.m_Color,l.m_Intensity });
	}


	void Light::RemovePLight(GameObject& go)
	{
	}
	
}