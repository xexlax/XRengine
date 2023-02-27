#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Resources/Shader.h"
#include "xre\ECS\GameObject.h"
#include "xre\ECS\Components.h"
namespace XRE {
	struct PointLight {
		glm::vec3 m_Position;
		glm::vec3 m_Color;
		float m_Intensity;

		PointLight(glm::vec3 pos, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 4.0f) {
			m_Position = pos;
			m_Color = color;
			m_Intensity = intensity;
		}

	};

	struct DirectionalLight {
		glm::vec3 m_Direction;
		glm::vec3 m_Color;
		float m_Intensity;


		DirectionalLight(glm::vec3 dir, glm::vec3 color= glm::vec3(1.0f,1.0f,1.0f), float intensity = 2.0f) {

			m_Direction = dir;
			m_Color = color;
			m_Intensity = intensity;
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
		void SetDirLight(GameObject& go);
		void ClearPLights();
		void SetPLight(GameObject& go,int i);

		void RemovePLight(GameObject& go);
		DirectionalLightComponent getDirLight() { return m_DirectionalLight; };
		PointLightComponent getPointLight(int idx = 0) { return m_PointLights[idx]; };
	private:

		DirectionalLightComponent m_DirectionalLight;
		std::vector<PointLightComponent> m_PointLights;


	};
}