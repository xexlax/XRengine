#pragma once
#include "xre\Core\Macros.h"

#include "xre\Resource\Model.h"
#include <string>
namespace XRE {

	//ECS中的组件只包含数据不包含方法。
	class Component
	{
	public:
		std::string m_Name;
		bool m_Active=true;

	};

	class TransformComponent : public Component {
	public:
		std::string m_Name= "Transform";
		glm::mat4 m_Transform{ 1.0f };

	/*	glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f);*/

		TransformComponent* m_parent = nullptr;


		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: m_Transform(transform) {}

		operator glm::mat4& () { return m_Transform; }
		operator const glm::mat4& () const { return m_Transform; }
	};

	class MeshRendererComponent : public Component {
	public:
		std::string m_Name = "MeshRenderer";
		Ref<Model> m_Model;
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(Ref<Model> model)
			: m_Model(model) {}

		operator Ref<Model>& () { return m_Model; }
		operator const Ref<Model>& () const { return m_Model; }
	};

	class PointLightComponent : public Component {
	public:
		std::string m_Name = "Point Light";
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Color = glm::vec3(1.0f);
		float m_Intensity=1.0f;
		float m_ShadowCasting = false;

		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(const glm::vec3& color, const float& intensity=1.0f)
			:m_Color(color), m_Intensity(intensity) {}


	};

	class DirectionalLightComponent : public Component {
	public:
		std::string m_Name = "Directional Light";

		glm::vec3 m_Color;
		float m_Intensity;
		float m_ShadowCasting = true;
		glm::vec3 m_Direction = glm::vec3(1.0f, -1.0f, 1.0f);
		DirectionalLightComponent() = default;
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent(const glm::vec3 & color = glm::vec3(1.0f), const float& intensity = 1.0f)
			:m_Color(color), m_Intensity(intensity) {}
	};



	class CameraComponent: public Component {
	public:
		std::string m_Name = "Camera";
		glm::mat4 m_Projection;
		bool m_FixedAspectRatio=false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: m_Projection(projection) {}

	};


}

