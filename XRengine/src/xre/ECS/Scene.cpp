
#include "pch.h"
#include "Scene.h"

#include "xre\ECS\Components.h"
#include "xre\ECS\GameObject.h"
#include "xre\Renderer\Renderer3D.h"
namespace XRE{
	

	GameObject Scene::CreateGameObject(const std::string& Name)
	{
		GameObject go = { m_Registry.create(), this, Name };
		go.AddComponent<TransformComponent>();
		return go;
	}

	void Scene::OnUpdate(TimeStep ts)
	{


		auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
		
		
		for (auto entity : group)
		{
			auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
			Renderer3D::DrawModel(meshrenderer,transform);
		}
	}


}