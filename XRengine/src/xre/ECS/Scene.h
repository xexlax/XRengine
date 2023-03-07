#pragma once

#include "entt\entt.hpp"
#include "xre/Core/Timestep.h"
#include "xre\ECS\Components.h"
#include <xre\Renderer\Camera\EditorCamera.h>
#include "xre\Physics\PhysicsScene.h"
#include <cereal\archives\json.hpp>

namespace XRE {

	enum SceneStatus {
		Editing, Runtime
	};
	class GameObject;
	

	class Scene
	{
	friend class GameObject;
	friend class ScenePanel;


	public:
		Scene();
		~Scene() = default;
		//在场景中新建物体
		GameObject CreateGameObject(const std::string& Name);
		//删除物体
		void Destroy(GameObject go);
		void OnViewportResize(uint32_t width, uint32_t height);
		
		
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_Name);
		}


	
		//整个场景的更新流程
		void OnUpdateRuntime(TimeStep ts);
		void OnUpdateEditing(TimeStep ts, EditorCamera& ec);

		std::pair<CameraComponent, TransformComponent> GetPrimaryCamera();




		entt::registry m_Registry;

		std::string GetName()const { return m_Name; };
		void SetName(const std::string& name) { m_Name = name; };

		void Serialize(const std::string& filepath);
		void Save() { Serialize(m_FilePath); }
		void Deserialize(const std::string& filepath);

	private:
		template<typename T>
		void OnComponentAdded(GameObject go, T& component);

	private:
		uint32_t m_ViewportWidth, m_ViewportHeight;
		std::string m_Name= "New Scene";
		std::string m_FilePath;



		XRef<PhysicsScene> m_PhysicsScene;
		

		void UpdateLighting();
		void UpdateNativeScripting(TimeStep ts);
		void UpdatePhysics();
		void UpdateRendering(XRef<Camera> c);

		


		
	};

	

}