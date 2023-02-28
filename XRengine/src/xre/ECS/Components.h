#pragma once
#include "xre\Core\Macros.h"

#include "xre\Resource\Model.h"
#include "xre\Renderer\Camera\Camera.h"
#include <string>
namespace XRE {

	//ECS中的组件只包含数据不包含方法。
	class Component
	{
	public:
		std::string m_Name;
		bool m_Active=true;

	};

	class NameComponent : public Component {
	public:
		std::string m_Name = "Name";
		std::string m_ObjName = "gameobject";

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string & name)
			: m_ObjName(name) {}


		operator std::string& () { return m_ObjName; }
		operator const std::string& () const { return m_ObjName; }
	};

	class TransformComponent : public Component {
	public:
		std::string m_Name= "Transform";
		glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent* m_parent = nullptr;


		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: m_Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), m_Translation)
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), m_Scale);
		}

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
		DirectionalLightComponent() = default;
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent(const glm::vec3 & color = glm::vec3(1.0f), const float& intensity = 1.0f)
			:m_Color(color), m_Intensity(intensity) {}
	};



	class CameraComponent: public Component {
	public:
		std::string m_Name = "Camera";
		Ref<Camera> m_Camera;
		bool m_FixedAspectRatio=false;
		bool m_Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			:m_Camera(std::make_shared<Camera>(projection)) {}
		CameraComponent(const CameraType& type)
			:m_Camera(std::make_shared<Camera>(type)) {}
	};


}

