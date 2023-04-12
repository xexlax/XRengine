#pragma once
#include "entt\entt.hpp"
#include "Scene.h"
#include "Components.h"
#include <cereal\archives\json.hpp>
namespace XRE {
	//不要给GameObj加上除了Component以外的任何属性，
	//因为GO的获取只是从对象池中读取一个编号，其所有的属性由component决定，类似于关系型数据库
	//GO的实际数据很少，可以直接作为临时储存对象或者传参
	class GameObject {
	public:
		GameObject()=default;
		GameObject(entt::entity entityhandle, Scene* scene);

		GameObject(const GameObject& other) = default;
		~GameObject()=default;

		
		entt::entity GetEntityHandle()const { return m_Entity; }

		template <class Archive>
		void save(Archive& archive) const {
			{
				auto ngo = GameObject(m_Entity, m_Scene);
				if (ngo.HasComponent<NameComponent>()) {
					ComponentType c("Name");
					archive(c);
					archive(ngo.GetComponent<NameComponent>());
				}

				if (ngo.HasComponent<TransformComponent>()) {
					ComponentType c("Transform");
					archive(c);
					archive(ngo.GetComponent<TransformComponent>());
				}

				if (ngo.HasComponent<MeshRendererComponent>()) {
					ComponentType c("Mesh Renderer");
					archive(c);
					archive(ngo.GetComponent<MeshRendererComponent>());
				}

				if (ngo.HasComponent<PointLightComponent>()) {
					ComponentType c("Point Light");
					archive(c);
					archive(ngo.GetComponent<PointLightComponent>());
				}

				if (ngo.HasComponent<DirectionalLightComponent>()) {
					ComponentType c("Directional Light");
					archive(c);
					archive(ngo.GetComponent<DirectionalLightComponent>());
				}

				if (ngo.HasComponent<CameraComponent>()) {
					ComponentType c("Camera");
					archive(c);
					archive(ngo.GetComponent<CameraComponent>());
				}
				if (ngo.HasComponent<AnimatorComponent>()) {
					ComponentType c("Animator");
					archive(c);
					archive(ngo.GetComponent<AnimatorComponent>());
				}
				if (ngo.HasComponent<RigidBodyComponent>()) {
					ComponentType c("Rigid Body");
					archive(c);
					archive(ngo.GetComponent<RigidBodyComponent>());
				}
				if (ngo.HasComponent<RayComponent>()) {
					ComponentType c("Ray");
					archive(c);
					archive(ngo.GetComponent<RayComponent>());
				}
				if (ngo.HasComponent<BluePrintComponent>()) {
					ComponentType c("BluePrint");
					archive(c);
					archive(ngo.GetComponent<BluePrintComponent>());
				}
				if (ngo.HasComponent<AudioSourceComponent>()) {
					ComponentType c("AudioSource");
					archive(c);
					archive(ngo.GetComponent<AudioSourceComponent>());
				}
				if (ngo.HasComponent<AudioListenerComponent>()) {
					ComponentType c("AudioListener");
					archive(c);
					archive(ngo.GetComponent<AudioListenerComponent>());
				}

				if (ngo.HasComponent<TransformComponent>()) {
					TransformComponent tc = ngo.GetComponent<TransformComponent>();
					if (!tc.children.empty()) {
						{
							ComponentType c("PUSHSTACK");
							archive(c);
						}
						for (auto e : tc.children) {
							GameObject go(e, m_Scene);
							archive(go);
						}
						{
							GameObject go = m_Scene->CreateGameObject("####ThePoper####");
							archive(go);
							m_Scene->Destroy(go);
						}
					}

				}
				ComponentType c("EndObj");
				archive(c);
			}
		}
		
		template <class Archive>
		void load(Archive& archive) {
			ComponentType n("0");
			auto go = GameObject(m_Entity, m_Scene);
			while (n.m_Name != "EndObj") {

				archive(n);
				if (n.m_Name == "EndObj") {
					break;
				}
				if (n.m_Name == "Name") {
					NameComponent& tc = go.GetComponent<NameComponent>();
					archive(tc);
				}

				if (n.m_Name == "Transform") {
					TransformComponent& tc = go.GetComponent<TransformComponent>();
					archive(tc);
				}
				if (n.m_Name == "Mesh Renderer") {
					MeshRendererComponent& tc = go.AddComponent<MeshRendererComponent>();
					archive(tc);
				}
				if (n.m_Name == "Point Light") {
					PointLightComponent& tc = go.AddComponent<PointLightComponent>();
					archive(tc);
				}
				if (n.m_Name == "Directional Light") {
					DirectionalLightComponent& tc = go.AddComponent<DirectionalLightComponent>();
					archive(tc);
				}
				if (n.m_Name == "Camera") {
					CameraComponent& tc = go.AddComponent<CameraComponent>(Perspective);
					archive(tc);
				}
				if (n.m_Name == "Animator") {
					AnimatorComponent& tc = go.AddComponent<AnimatorComponent>();
					archive(tc);
				}
				if (n.m_Name == "Rigid Body") {
					RigidBodyComponent& tc = go.AddComponent<RigidBodyComponent>();
					archive(tc);
				}
				if (n.m_Name == "Ray") {
					RayComponent& tc = go.AddComponent<RayComponent>();
					archive(tc);
				}
				if (n.m_Name == "BluePrint") {
					BluePrintComponent& tc = go.AddComponent<BluePrintComponent>();
					archive(tc);
				}
				if (n.m_Name == "AudioSource") {
					AudioSourceComponent& tc = go.AddComponent<AudioSourceComponent>();
					archive(tc);
				}
				if (n.m_Name == "AudioListener") {
					AudioListenerComponent& tc = go.AddComponent<AudioListenerComponent>();
					archive(tc);
				}

				if (n.m_Name == "PUSHSTACK") {

					while (n.m_Name != "POPSTACK")
					{
						
						GameObject sgo = m_Scene->CreateGameObject("s");
						archive(sgo);
						if (sgo.GetComponent<NameComponent>().m_ObjName == "####ThePoper####") {
							m_Scene->Destroy(sgo);
							break;
						}
						else {
							sgo.SetParentAbs(&go);
						}
							
						
					}		
					
				}
			}

			
		};

		

		void SetParent(GameObject* parent);
		void SetParent(TransformComponent& parent);
		void SetParentAbs(GameObject* parent);
		GameObject Duplicate();
		

		//重载：强制类型转换和逻辑
		operator bool() const { return m_Entity != entt::null; }
		operator entt::entity() const { return m_Entity; }
 		operator uint32_t() const { return (uint32_t)m_Entity; }

		bool operator==(const GameObject& other) const
		{
			return m_Entity == other.m_Entity && m_Scene == other.m_Scene;
		}

		bool operator!=(const GameObject& other) const
		{
			return m_Entity != other.m_Entity && m_Scene == other.m_Scene;
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			XRE_CORE_ASSERT(!HasComponent<T>(), "对象已存在组件!");
			T& component= m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			XRE_CORE_ASSERT(HasComponent<T>(), "对象不存在组件!");
			return m_Scene->m_Registry.get<T>(m_Entity);
		}


		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Entity);
		}

		template<typename T>
		void RemoveComponent()
		{
			XRE_CORE_ASSERT(HasComponent<T>(), "对象不存在组件!");
			m_Scene->m_Registry.remove<T>(m_Entity);
		}
		std::string GetName() { return GetComponent<NameComponent>(); }
		void SetName(const std::string& name) { GetComponent<NameComponent>() = name; }
		Scene* GetScene() const { return m_Scene; }

	private:

		

		entt::entity m_Entity{ entt::null };
		Scene* m_Scene= nullptr;
		
	};
	
}