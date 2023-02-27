#pragma once

#include "entt\entt.hpp"
#include "xre/Core/Timestep.h"


namespace XRE {
	class GameObject;
	class Scene
	{

		
	public:
		Scene() = default;
		~Scene() = default;
		GameObject CreateGameObject(const std::string& Name);
		void OnUpdate(TimeStep ts);
		entt::registry m_Registry;

		
	};

}