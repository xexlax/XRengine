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
		bool m_ShadowCasting= false;
		float m_Intensity;

		PointLight(glm::vec3 pos= glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 4.0f) {
			m_Position = pos;
			m_Color = color;
			m_Intensity = intensity;
		}

	};

	struct DirectionalLight {
		glm::vec3 m_Direction;
		glm::vec3 m_Rotation;
		glm::vec3 m_Color;
		bool m_ShadowCasting= true;
		float m_Intensity;


		DirectionalLight(glm::vec3 dir, glm::vec3 color= glm::vec3(1.0f,1.0f,1.0f), float intensity = 1.0f) {

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
	//整个当前场景的灯光布局
	class LightSystem {


	public:
		LightSystem();
		~LightSystem() {};
		void Draw(XRef<Shader> shader);
		void SetDirLight(TransformComponent t, DirectionalLightComponent l);
		void ClearPLights();
		void AddPLight(TransformComponent t, PointLightComponent l);
		size_t getPLightCount() {
			return m_PointLights.size();
		}
		void RemovePLight(GameObject& go);
		DirectionalLight getDirLight() { return m_DirectionalLight; };
		PointLight getPointLight(int idx = 0) { 
			return m_PointLights[idx]; };
	private:

		DirectionalLight m_DirectionalLight;
		std::vector<PointLight> m_PointLights;


	};
}