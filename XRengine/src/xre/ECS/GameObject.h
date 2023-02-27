#pragma once
#include "entt\entt.hpp"
#include "xre\ECS\Scene.h"

namespace XRE {

	class GameObject {
	public:
		GameObject()=default;
		GameObject(entt::entity entityhandle, Scene* scene ,const std::string& name="newObj");

		GameObject(const GameObject& other) = default;
		~GameObject()=default;

		std::string GetName()const { return m_Name; }
		entt::entity GetEntityHandle()const { return m_Entity; }
		void SetName(const std::string& name) { m_Name = name; }

		operator bool() const { return m_Entity != entt::null; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			XRE_CORE_ASSERT(!HasComponent<T>(), "对象已存在组件!");
			return m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
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

	private:
		std::string m_Name;
		entt::entity m_Entity{ entt::null };
		Scene* m_Scene= nullptr;
	};
}