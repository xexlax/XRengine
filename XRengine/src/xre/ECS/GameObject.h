#pragma once
#include "entt\entt.hpp"
#include "Scene.h"
#include "Components.h"

namespace XRE {

	class GameObject {
	public:
		GameObject()=default;
		GameObject(entt::entity entityhandle, Scene* scene);

		GameObject(const GameObject& other) = default;
		~GameObject()=default;

		
		entt::entity GetEntityHandle()const { return m_Entity; }
		

		//���أ�ǿ������ת�����߼�
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
			XRE_CORE_ASSERT(!HasComponent<T>(), "�����Ѵ������!");
			T& component= m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			XRE_CORE_ASSERT(HasComponent<T>(), "���󲻴������!");
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
			XRE_CORE_ASSERT(HasComponent<T>(), "���󲻴������!");
			m_Scene->m_Registry.remove<T>(m_Entity);
		}
		std::string GetName() { return GetComponent<NameComponent>(); }
		void SetName(const std::string& name) { GetComponent<NameComponent>() = name; }

	private:
		//��Ҫ��GameObj���ϳ���Component������κ����ԣ���ΪGO�Ļ�ȡֻ�Ǵ�ECS���ж�ȡ��ţ������е�������component����
		
		entt::entity m_Entity{ entt::null };
		Scene* m_Scene= nullptr;
	};
}