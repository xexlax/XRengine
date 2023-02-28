#include "pch.h"
#include "GameObject.h"

namespace XRE {

	GameObject::GameObject(entt::entity entityhandle, Scene* scene):
		m_Entity(entityhandle),m_Scene(scene)
	{
	}
	
	
}