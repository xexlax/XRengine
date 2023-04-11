#pragma once
#include "xre\Core\Macros.h"
#include "xre\Resource\ResourceManager.h"
#include "xre\Resource\Model.h"
#include "xre\Renderer\Camera\Camera.h"
#include "xre\Physics\PhysicsShape.h"
#include "xre\Physics\PhysicsMaterial.h"
#include "xre\Physics\PhysicsMotion.h"
#include "xre\Physics\Collision.h"

#include "xre\Audio\Audio.h"

#include "xre\BluePrint\BluePrint.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <entt/entt.hpp>

//添加一个新的Component之后：
//1.添加序列化方法，在Scene中序列化函数中加入
//2.在Editor-Propertypanel中编写DrawLayout
//3.在Scene的Update函数中加入具体的功能

using namespace entt;
namespace XRE {

	//ECS中的组件只包含数据不包含方法。
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

		TransformComponent() :Component(u8"变换") {};
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			:Component(u8"变换") , m_Translation(translation) {}

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

		std::vector<XRef<Material>> m_Materials;

		
		MeshRendererComponent() :Component(u8"模型渲染器") {};
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(XRef<Model> model)
			:Component(u8"模型渲染器"), m_Model(model), m_Materials(model->m_defaultMaterials) {
			m_ModelPath = model->getPath();
			if (m_Materials.empty())m_Materials.push_back(nullptr);
		};
		MeshRendererComponent(const std::string& path)
			:m_Model(ResourceManager::GetModel(path))
			, m_ModelPath(path)
		{
			m_Materials = m_Model->m_defaultMaterials;
			if (m_Materials.empty())m_Materials.push_back(nullptr);
		};

		XRef<Material> GetMaterial(uint32_t i = 0) const {
			if (i >= m_Materials.size() || m_Materials[i] == nullptr) {
				if (i >= m_Model->m_defaultMaterials.size()) {
					return nullptr;
				}
				return m_Model->m_defaultMaterials[i];
			}
			return m_Materials[i];
		};

		operator XRef<Model>& () { return m_Model; }
		operator const XRef<Model>& () const { return m_Model; }

		template <class Archive>
		void save(Archive& ar)const
		{
			ar(m_Active, m_ModelPath);
			std::vector<std::string> f;
			for (auto x : m_Materials) {
				if(x)
				f.push_back(x->GetPath());
				else f.push_back("");
			}
			ar(f);
			
		}

		template <class Archive>
		void load(Archive& ar)
		{
			ar(m_Active, m_ModelPath );
			m_Model = ResourceManager::GetModel(m_ModelPath);
			std::vector<std::string> f;
			ar(f);
			int i = 0;
			m_Materials.clear();

			for (auto x : f){
				if (x != "")
					m_Materials.push_back(ResourceManager::GetMaterial(x));
				else
					m_Materials.push_back(nullptr);
			}
			//m_Materials.std::resize(max(m_Materials.size(), m_Model->m_defaultMaterials.size());
		
		}

	};

	class PointLightComponent : public Component {
	public:
		
		glm::vec3 m_Color = glm::vec3(1.0f);
		float m_Intensity=1.0f;
		bool m_ShadowCasting = false;

		PointLightComponent() :Component(u8"点光源") {};
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(const glm::vec3& color, const float& intensity=1.0f)
			:Component(u8"点光源"),m_Color(color), m_Intensity(intensity) {}

		

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
		DirectionalLightComponent() :Component(u8"平行光") {};
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent(const glm::vec3 & color, const float& intensity = 1.0f)
			:Component(u8"平行光") ,m_Color(color), m_Intensity(intensity) {}
	

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
		

		CameraComponent() :Component(u8"摄像机") {};
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			:Component(u8"摄像机"),m_Camera(XMakeRef<Camera>(projection)) {}
		CameraComponent(const CameraType& type)
			:Component(u8"摄像机"), m_Camera(XMakeRef<Camera>(type)) {}


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
		vector<uint32_t> m_HitObjs;
		vector<PhysicsHitInfo> m_HitResults;



		bool m_ShowShape = false;

		enum RigidBodyMotion
		{
			Static, Kinematic, Dynamic, Trigger
		} 
		m_MotionType = RigidBodyMotion::Static;




		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active, m_MotionType, m_Shape, m_PhysicsMaterial);
		}

		RigidBodyComponent() :Component(u8"刚体") {};
		

		

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

		RayComponent() :Component(u8"射线") {};
	};

	class BluePrintComponent :public Component{
	public:
		XRef<BluePrint> m_BluePrint;
		XRef<BluePrintProperties> m_BluePrintProperties;
		std::string m_BluePrintPath;

		void SetBP(XRef<BluePrint> bp) {
			m_BluePrint = bp;
			m_BluePrintProperties->Update(bp->GetDefaultProperties());
			m_BluePrintPath =  m_BluePrint->GetFileName();
		}

		template <class Archive>
		void save(Archive& ar) const
		{
			ar(m_BluePrintPath,*m_BluePrintProperties);

		}
		template <class Archive>
		void load(Archive& ar) 
		{

			ar(m_BluePrintPath);
			m_BluePrint = ResourceManager::GetBluePrint(m_BluePrintPath);
			ar(*m_BluePrintProperties);
		}

		BluePrintComponent() :Component(u8"蓝图") {
			m_BluePrintProperties = XMakeRef<BluePrintProperties>();
		};
	};

	class AudioSourceComponent : public Component {
	public:

		 
		std::vector<std::string> AudioClipPaths;
		std::vector<XRef<AudioClip>> AudioClips;
		
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active,AudioClipPaths);

		}

		AudioSourceComponent() :Component(u8"声源") {};
		
	};

	class AudioListenerComponent : public Component {
	public:


		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Active);

		}

		AudioListenerComponent() :Component(u8"声音接收器") {};

	};
}

