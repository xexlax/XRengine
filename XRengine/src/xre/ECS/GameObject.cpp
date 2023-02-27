#include "pch.h"
#include "GameObject.h"

namespace XRE {

	GameObject::GameObject(entt::entity entityhandle, Scene* scene, const std::string& name):
		m_Entity(entityhandle),m_Scene(scene),m_Name(name)
	{
	}
	
	
}