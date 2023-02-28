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
			auto& openglshader = std::dynamic_pointer_cast<OpenGLShader>(shader);

			openglshader->Bind();
			openglshader->SetFloat3("d_light.direction", m_DirectionalLight.m_Direction);
			openglshader->SetFloat("d_light.intensity", m_DirectionalLight.m_Intensity);
			openglshader->SetFloat3("d_light.color", m_DirectionalLight.m_Color);

			int index = 0;
			for (auto p : m_PointLights) {
				
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
		TransformComponent t = go.GetComponent<TransformComponent>();
		DirectionalLightComponent l  = go.GetComponent<DirectionalLightComponent>();
		
		m_DirectionalLight = { glm::vec4(0,0,-1,0) * t.GetTransform() ,l.m_Color,l.m_Intensity };
		m_DirectionalLight.m_Rotation = t.m_Rotation;
	}
	void Light::ClearPLights()
	{
		m_PointLights.clear();
	}

	void Light::SetPLight(GameObject& go,int i)
	{
		TransformComponent t = go.GetComponent<TransformComponent>();
		PointLightComponent l = go.GetComponent<PointLightComponent>();
		m_PointLights[i] = {t.m_Translation,l.m_Color,l.m_Intensity };
	}

	void Light::RemovePLight(GameObject& go)
	{
	}
	
}