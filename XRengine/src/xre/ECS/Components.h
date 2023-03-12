#pragma once
#include "xre\Core\Macros.h"
#include "xre\Resource\ResourceManager.h"
#include "xre\Resource\Model.h"
#include "xre\Renderer\Camera\Camera.h"
#include "xre\Physics\PhysicsShape.h"
#include "xre\Physics\PhysicsMaterial.h"
#include "xre\Physics\PhysicsMotion.h"
#include "xre\Physics\Collision.h"
#include <cereal/archives/json.hpp>

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <entt/entt.hpp>

//���һ���µ�Component֮��
//1.������л���������Scene�����л������м���
//2.��Editor-Propertypanel�б�дDrawLayout
//3.��Scene��Update�����м������Ĺ���

using namespace entt;
namespace XRE {

	//ECS�е����ֻ�������ݲ�����������
	struct ComponentType {

		std::string m_Name;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Name);
		}

		ComponentType(const std::string& name) {
			m_Name = name;
		}
	};
	class Component
	{
	public:
		std::string m_Name;
		bool m_Active=true;

		Component() = default;
		Component(const std::string& name) {
			m_Name = name;
		}

		

	};
	
	

	class NameComponent : public Component {
	public:
		std::string m_Name = "Name";
		std::string m_ObjName = "gameobject";

		/*static void CreateReflection() {
			meta<NameComponent>().type("NameComponent"_hs)
				.ctor<const std::string&>()
				.data<&NameComponent::m_Active>("active"_hs)
				.prop<std::string>("active"_hs, "active")
				.data<&NameComponent::m_ObjName>("name"_hs)
				.prop<std::string>("name"_hs, "name");
				
		}*/

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string & name)
			: m_ObjName(name) {}

		operator std::string& () { return m_ObjName; }
		operator const std::string& () const { return m_ObjName; }

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active,m_ObjName);
		}
	};



	class TransformComponent : public Component {
		
	public:
		
		
		
		
		glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };


		template <class Archive>
		void save(Archive& ar) const
		{
			ar(m_Active, m_Translation.x,m_Translation.y,m_Translation.z,
				m_Rotation.x, m_Rotation.y, m_Rotation.z, 
				m_Scale.x, m_Scale.y, m_Scale.z);
		}

		template <class Archive>
		void load(Archive& ar)
		{
			ar(m_Active, m_Translation.x, m_Translation.y, m_Translation.z,
				m_Rotation.x, m_Rotation.y, m_Rotation.z,
				m_Scale.x, m_Scale.y, m_Scale.z);
		}

		TransformComponent* parent=nullptr;
		std::vector<entt::entity> children;

		TransformComponent() :Component("Transform") {};
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			:Component("Transform") , m_Translation(translation) {}

		glm::mat4 GetGlobalTransform() const
		{
			
			if (parent) {
				glm::mat4 result = parent->GetGlobalTransform() * glm::translate(glm::mat4(1.0f), m_Translation)
					* glm::toMat4(glm::quat(glm::radians(m_Rotation)))
					* glm::scale(glm::mat4(1.0f), m_Scale);
					
					
					 
				return result;
			}
			else {
				glm::mat4 result = glm::translate(glm::mat4(1.0f), m_Translation)
					* glm::toMat4(glm::quat(glm::radians(m_Rotation)))
					* glm::scale(glm::mat4(1.0f), m_Scale);
				return result;
			}
		}

		glm::mat4 GetLocalTransform() const
		{
			glm::mat4 result = glm::translate(glm::mat4(1.0f), m_Translation)
				*glm::toMat4(glm::quat(glm::radians(m_Rotation)))
				* glm::scale(glm::mat4(1.0f), m_Scale);
				
				
			return result;
		}

		glm::vec3 GetGlobalTranslation() const {

			if (parent) return parent->GetGlobalTranslation() + m_Translation;
			else return m_Translation;

		}

		glm::quat GetGlobalRotation() const {

			if (parent) return parent->GetGlobalRotation()* glm::quat(glm::radians(m_Rotation));
			else return glm::quat(glm::radians(m_Rotation));

		}
		glm::vec3 GetGlobalEuler() const {
			if (parent) return parent->GetGlobalEuler() + m_Rotation;
			else return m_Rotation;
		}

		glm::vec3 GetGlobalDirection() const {
			return glm::normalize(glm::vec4(0, 0, 1, 0) * glm::inverse(glm::toMat4(GetGlobalRotation())));
		}

		glm::vec3 GetGlobalScale() const {

			if (parent) return parent->GetGlobalScale() * m_Scale;
			else return m_Scale;

		}

		void SetGlobalTranslation(glm::vec3 trans) {
			if (parent) {
				m_Translation = trans - parent->GetGlobalTranslation();
			}
			else
				m_Translation = trans;
		}

		void SetGlobalRotation(glm::vec3 rot) {
			if (parent) {
				m_Rotation = rot - parent->GetGlobalEuler();
			}
			else
				m_Rotation = rot;
		}

		void SetGlobalScale(glm::vec3 scale) {
			if (parent) {
				m_Scale = scale / parent->GetGlobalScale();
			}
			else
				m_Scale = scale;
		}

	


	};

	class MeshRendererComponent : public Component {
	public:
		std::string m_ModelPath;
		bool m_ShadowCasting = true;
		XRef<Model> m_Model;
		MeshRendererComponent() :Component("Mesh Renderer") {};
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(XRef<Model> model)
			:Component("Mesh Renderer"), m_Model(model) {
			m_ModelPath = model->getPath();
		}
		MeshRendererComponent(const std::string& path)
			: m_Model(ResourceManager::GetModel(path)),m_ModelPath(path) {}

		operator XRef<Model>& () { return m_Model; }
		operator const XRef<Model>& () const { return m_Model; }

		template <class Archive>
		void save(Archive& ar)const
		{
			ar(m_Active, m_ModelPath);
			
		}

		template <class Archive>
		void load(Archive& ar)
		{
			ar(m_Active, m_ModelPath);
			m_Model = ResourceManager::GetModel(m_ModelPath);
		}

	};

	class PointLightComponent : public Component {
	public:
		
		glm::vec3 m_Color = glm::vec3(1.0f);
		float m_Intensity=1.0f;
		bool m_ShadowCasting = false;

		PointLightComponent() :Component("Point Light") {};
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(const glm::vec3& color, const float& intensity=1.0f)
			:Component("Point Light"),m_Color(color), m_Intensity(intensity) {}

		

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active, m_Color.r,m_Color.g,m_Color.b,m_Intensity);
		
		}
	
	};

	class DirectionalLightComponent : public Component {
	public:
		

		glm::vec3 m_Color=glm::vec3(1.0f);
		float m_Intensity=1.0f;
		bool m_ShadowCasting = true;
		DirectionalLightComponent() :Component("Directional Light") {};
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent(const glm::vec3 & color, const float& intensity = 1.0f)
			:Component("Directional Light") ,m_Color(color), m_Intensity(intensity) {}
	

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active, m_Color.r, m_Color.g, m_Color.b, m_Intensity, m_ShadowCasting);

		}
	};



	class CameraComponent: public Component {
	public:

		XRef<Camera> m_Camera;
		bool m_FixedAspectRatio=false;
		bool m_Primary = true;
		

		CameraComponent() :Component("Camera") {};
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			:Component("Camera"),m_Camera(XMakeRef<Camera>(projection)) {}
		CameraComponent(const CameraType& type)
			:Component("Camera"), m_Camera(XMakeRef<Camera>(type)) {}


		template <class Archive>
		void save(Archive& ar)const
		{
			ar(m_Active, m_FixedAspectRatio , m_Primary ,
				m_Camera->m_Type,m_Camera->m_OrthographicFar, m_Camera->m_OrthographicNear,
				m_Camera->m_OrthographicSize, m_Camera->m_PerspectiveFar,
			m_Camera->m_PerspectiveNear,m_Camera->m_PerspectiveFovy);

		}

		template <class Archive>
		void load(Archive& ar)
		{
			m_Camera = make_shared<Camera>();
			ar(m_Active, m_FixedAspectRatio, m_Primary,
				m_Camera->m_Type, m_Camera->m_OrthographicFar, m_Camera->m_OrthographicNear,
				m_Camera->m_OrthographicSize, m_Camera->m_PerspectiveFar,
				m_Camera->m_PerspectiveNear, m_Camera->m_PerspectiveFovy);
			m_Camera->RecalculateProjectionMatrix();
			
		}
		
	};

	class AnimatorComponent : public Component {
	public:
		
		glm::vec3 m_Pivot = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Axis = { 0.0f, 1.0f, 0.0f };
		float m_Radius = 1.0f;
		float m_AngleSpeed = 1.0f;
		float m_Phase = 0.0f;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active, m_Pivot.x, m_Pivot.y,m_Pivot.z
				,m_Axis.x,m_Axis.y,m_Axis.z
				, m_Radius,m_AngleSpeed,m_Phase);
		}

		AnimatorComponent():Component("Animator") {};
		AnimatorComponent(float r, float s,float f=0) :Component("Animator"),m_Radius(r), m_AngleSpeed(s), m_Phase(f) {

		};


	};

	class RigidBodyComponent : public Component {
	public:
		

		uint32_t m_PhysicObj=0;

		//bool m_UseIndependentShapeTransform = false;

		PhysicsShape m_Shape;
		PhysicsMaterial m_PhysicsMaterial;
		PhysicsMotion m_Motion;





		bool m_ShowShape = false;

		enum RigidBodyMotion
		{
			Static, Kinematic, Dynamic
		} 
		m_MotionType = RigidBodyMotion::Static;




		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active, m_MotionType, m_Shape, m_PhysicsMaterial);
		}

		RigidBodyComponent() :Component("Rigid Body") {};
		

		

		void SetMotion(int i) { 
			m_MotionType = RigidBodyComponent::RigidBodyMotion(i); 
		};

	};


	class RayComponent :public Component{
	public:
		float m_MaxLength = 10.0f;
		bool m_Display = false;
		bool m_ShapeSweep = false;

		PhysicsShape m_SweepShape;

		vector<PhysicsHitInfo> m_HitResults;
		vector<uint32_t> m_HitObjs;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active,m_MaxLength,m_Display,m_ShapeSweep);
			if (m_ShapeSweep) {
				ar(m_SweepShape);
			}
		}

		RayComponent() :Component("Ray") {};
	};
}

