#pragma once

#include "entt\entt.hpp"
#include "xre/Core/Timestep.h"
#include "xre\ECS\Components.h"
#include <xre\Renderer\Camera\EditorCamera.h>
#include "xre\Physics\PhysicsScene.h"
#include <cereal\archives\json.hpp>
#include "xre\BluePrint\BluePrint.h"

namespace XRE {

	enum SceneStatus {
		Editing, Runtime ,Paused
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

		GameObject CreateFromFile(std::string fp);
		//删除物体
		void Destroy(GameObject go);
		void OnViewportResize(uint32_t width, uint32_t height);
		void OnRuntimeBegin();
		void OnRuntimeEnd();
		
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

		std::string GetFilePath() const { return m_FilePath; };

		void Serialize(const std::string& filepath);
		void Save() { Serialize(m_FilePath); }
		void Load() { m_Registry.clear(); Deserialize(m_FilePath); }
		void Deserialize(const std::string& filepath);


		GameObject FindGOByPhysicBodyID(uint32_t pid);
		vector<GameObject> FindGOByName(std::string str);
		GameObject GetObj(uint32_t eid);

		bool m_PostProcessing = true;

		XRef<BluePrint> GetGlobalBluePrint() { return m_GlobalBluePrint; }
		XRef<BluePrintProperties> GetGlobalProperties() { return m_GlobalProperties; }
		XRef<PhysicsScene> m_PhysicsScene;
	private:
		template<typename T>
		void OnComponentAdded(GameObject go, T& component);

	private:
		uint32_t m_ViewportWidth, m_ViewportHeight;
		std::string m_Name = "New Scene";
		std::string m_FilePath = "";

		bool m_Runtime = false;

		
		
		XRef<BluePrint> m_GlobalBluePrint;
		XRef<BluePrintProperties> m_GlobalProperties;
		

		void ThreadRendering(TimeStep ts);
		void ThreadLogic(TimeStep ts);

		void UpdateNativeScripting(TimeStep ts);
		void UpdatePhysics(TimeStep ts);
		void UpdateAudio(TimeStep ts);
		void UpdateRendering(XRef<Camera> c);


		void UpdateLogic(TimeStep ts);

		


		
	};

	

}