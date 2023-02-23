#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Resources/Shader.h"
namespace XRE {
	struct PointLight {
		glm::vec3 m_Position;
		glm::vec3 m_Color;
		float m_Intensity;

		PointLight(glm::vec3 pos, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 10.0f) {
			m_Position = pos;
			m_Color = color;
			m_Intensity = intensity;
		}

	};

	struct DirectionalLight {
		glm::vec3 m_Direction;
		glm::vec3 m_Color;

		DirectionalLight(glm::vec3 dir, glm::vec3 color= glm::vec3(1.0f,1.0f,1.0f)) {

			m_Direction = dir;
			m_Color = color;
		}
		
	};

	struct SpotLight {
		glm::vec3 m_Position;
		glm::vec3 m_Direction;
		glm::vec3 m_Color;
		float m_RadiusAngle;
		float m_Intensity;
	};
	//Light System
	class Light {


	public:
		Light();
		~Light() {};
		void Draw(Ref<Shader> shader);
		void SetDirLight(Ref<DirectionalLight> d);
		void ClearPLights();
		void AddPLight(Ref<PointLight> p);
		Ref<DirectionalLight> getDirLight() { return m_DirectionalLight; };
		Ref<PointLight> getPointLight(int idx = 0) { return m_PointLights[idx]; };
	private:

		Ref<DirectionalLight> m_DirectionalLight;
		std::vector<Ref<PointLight>> m_PointLights;


	};
}