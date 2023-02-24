#include "pch.h"
#include "Entity.h"

namespace XRE {
	Entity::Entity()
	{
		OnCreate();
	}
	Entity::~Entity()
	{
		OnDestroy();
	}
	void Entity::OnCreate()
	{
	}
	void Entity::OnDestroy()
	{
	}
	void Entity::OnUpdate()
	{
		for (auto c : m_Components) {
			c->OnUpdate();
		}
	}
	void Entity::AddComponent(Ref<Component> c)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), c);
		if (it == m_Components.end()) {
			m_Components.push_back(c);
			c->OnAttach();
		}
			
	}
	void Entity::RemoveComponent(Ref<Component> c)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(),c);
		if (it != m_Components.end()) {
			m_Components.erase(it);
			c->OnDetach();
		}
			
	}
}