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
				//Math::DecomposeTransform(sc.GetGlobalTransform(), sc.m_Translation, sc.m_Rotation, sc.m_Scale);
				sc.m_Translation = sc.GetGlobalTranslation();
				sc.m_Rotation = sc.GetGlobalEuler();
				sc.m_Scale = sc.GetGlobalScale();
			}
			sc.parent = nullptr;
			
			return;
		}
		
		TransformComponent& pc = parent->GetComponent<TransformComponent>();
		if (sc.parent != &pc) {
			if (sc.parent)
				sc.parent->children.erase(find(sc.parent->children.begin(), sc.parent->children.end(), m_Entity));
			
			auto gt = sc.GetGlobalTranslation();
			auto gr = sc.GetGlobalEuler();
			auto gs = sc.GetGlobalScale();
			//Math::DecomposeTransform(glm::inverse(pc.GetGlobalTransform())*sc.GetGlobalTransform(), sc.m_Translation, sc.m_Rotation, sc.m_Scale);
			
			
			sc.parent = &pc;
			sc.SetGlobalTranslation(gt);
			sc.SetGlobalRotation(gr);
			sc.SetGlobalScale(gs);

			pc.children.push_back(m_Entity);
		}
	}

	void GameObject::SetParent(TransformComponent& pc)
	{
		
		TransformComponent& sc = GetComponent<TransformComponent>();
		
		if (sc.parent != &pc) {
			if (sc.parent)
				sc.parent->children.erase(find(sc.parent->children.begin(), sc.parent->children.end(), m_Entity));

			auto gt = sc.GetGlobalTranslation();
			auto gr = sc.GetGlobalEuler();
			auto gs = sc.GetGlobalScale();
			//Math::DecomposeTransform(glm::inverse(pc.GetGlobalTransform())*sc.GetGlobalTransform(), sc.m_Translation, sc.m_Rotation, sc.m_Scale);


			sc.parent = &pc;
			sc.SetGlobalTranslation(gt);
			sc.SetGlobalRotation(gr);
			sc.SetGlobalScale(gs);

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
		//输入输出要将生命周期完成之后再切换
		{
			
			//auto ngo = GameObject(m_Entity, m_Scene);
			cereal::JSONOutputArchive oarchive(fs);

			oarchive(*this);
		}

		{
			
			cereal::JSONInputArchive iarchive(fs);



			GameObject go = m_Scene->CreateGameObject(GetName() + u8" 副本");
			iarchive(go);
			go.GetComponent<NameComponent>().m_Name = GetName() + u8" 副本";
			return go;
		}

	}

	
	
}