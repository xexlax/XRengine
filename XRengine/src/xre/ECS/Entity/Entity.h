#pragma once

#include "xre\ECS\Components\Component.h"
#include <cstdint>
namespace XRE {
	class Entity {

		Entity();
		~Entity();

		void OnCreate();
		void OnDestroy();
		void OnUpdate();

		void AddComponent(Ref<Component> c);
		void RemoveComponent(Ref<Component> c);

		std::string GetName()const { return m_Name; }
		uint32_t GetId()const { return m_Id; }

		void SetName(const std::string& name) { m_Name = name; }

		template <typename T> Ref<T> GetComponent();


	private:
		std::vector<Ref<Component>> m_Components;
		std::string m_Name;
		uint32_t m_Id;

	};

	template<typename T>
	inline Ref<T> Entity::GetComponent()
	{
		for (auto c : m_Components) {
			if (typeid(*c) == typeid(T)) {
				return std::dynamic_pointer_cast<T>(c);
			}
		}
	}
}