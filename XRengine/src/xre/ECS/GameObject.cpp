#include "pch.h"
#include "GameObject.h"
#include "xre\Utils\Math.h"

#include "cereal\archives\json.hpp"

namespace XRE {

	GameObject::GameObject(entt::entity entityhandle, Scene* scene):
		m_Entity(entityhandle),m_Scene(scene)
	{
	}
	
	void GameObject::SetParent(GameObject* parent) {
		TransformComponent& sc = GetComponent<TransformComponent>();
		if (parent == nullptr) {
			if (sc.parent) {
				sc.parent->children.erase(find(sc.parent->children.begin(), sc.parent->children.end(), m_Entity));
				Math::DecomposeTransform(sc.GetTransform(), sc.m_Translation, sc.m_Rotation, sc.m_Scale);

			}
			sc.parent = nullptr;
			
			return;
		}
		
		TransformComponent& pc = parent->GetComponent<TransformComponent>();
		if (sc.parent != &pc) {
			if (sc.parent)
				sc.parent->children.erase(find(sc.parent->children.begin(), sc.parent->children.end(), m_Entity));
			
			Math::DecomposeTransform(glm::inverse(pc.GetTransform())*sc.GetTransform(), sc.m_Translation, sc.m_Rotation, sc.m_Scale);

			
			sc.parent = &pc;
			pc.children.push_back(m_Entity);
		}
	}

	void GameObject::SetParentAbs(GameObject* parent) {
		TransformComponent& sc = GetComponent<TransformComponent>();
		if (parent == nullptr) {
			if (sc.parent) {
				sc.parent->children.erase(find(sc.parent->children.begin(), sc.parent->children.end(), m_Entity));

			}
			sc.parent = nullptr;

			return;
		}

		TransformComponent& pc = parent->GetComponent<TransformComponent>();
		if (sc.parent != &pc) {
			if (sc.parent)
				sc.parent->children.erase(find(sc.parent->children.begin(), sc.parent->children.end(), m_Entity));

			sc.parent = &pc;
			pc.children.push_back(m_Entity);
		}
	}

	GameObject GameObject::Duplicate()
	{

		//std::stringstream ss;
		std::stringstream fs;
		//�������Ҫ�������������֮�����л�
		{
			
			//auto ngo = GameObject(m_Entity, m_Scene);
			cereal::JSONOutputArchive oarchive(fs);

			oarchive(*this);
		}

		{
			
			cereal::JSONInputArchive iarchive(fs);



			GameObject go = m_Scene->CreateGameObject(GetName() + u8" ����");
			iarchive(go);
			go.GetComponent<NameComponent>().m_Name = GetName() + u8" ����";
			return go;
		}

	}

	
	
}