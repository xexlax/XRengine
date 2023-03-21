#include "ScenePanel.h"
#include "../Commands/CommandManager.h"
#include "PanelsManager.h"
#include <ImGui\imgui.h>
namespace XRE{
	ScenePanel::ScenePanel(const XRef<Scene>& scene)
	{
		AttachToScene(scene);
	}
	void ScenePanel::AttachToScene(const XRef<Scene>& scene)
	{
		m_Scene = scene;
		m_Selected = GameObject(entt::null ,m_Scene.get());
		PanelsManager::GetBluePrintEditor()->m_BluePrint = scene->GetGlobalBluePrint();
	}
	void ScenePanel::Select(int ObjId)
	{
		GameObject go( entt::entity(ObjId) , m_Scene.get() );
		m_Selected = go;
		PanelsManager::GetPropertiesPanel()->Switch();
		
		
	}
	void ScenePanel::UnSelect()
	{
		m_Selected = GameObject(entt::null, m_Scene.get());
	}
	void ScenePanel::OnImGuiRender()

	{
		ImGui::Begin(u8"场景");
		bool opened = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, m_Scene->m_Name.c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragNodeObj"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject));
				GameObject sub = *(const GameObject*)payload->Data;
				sub.SetParent(nullptr);

			}
			ImGui::EndDragDropTarget();
		}
		
		if (opened)
		{
			m_Scene->m_Registry.each([&](auto entityID)
				{
					//shared_ptr.get 返回其raw pointer
					GameObject go{ entityID , m_Scene.get() };
					if (go.GetComponent<TransformComponent>().parent == nullptr)
						DrawGONode(go);
				});


			
			ImGui::TreePop();
		}

		

		
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_Selected = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem(u8"空对象")) {
				m_Scene->CreateGameObject(u8"空对象");
			}

			if (ImGui::BeginMenu(u8"预设体"))
			{
				if (ImGui::MenuItem(u8"立方体")) {

					auto go = m_Scene->CreateGameObject(u8"立方体");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Cube));
					CommandManager::Get().Command_CreateObj(go);


				}
				if (ImGui::MenuItem(u8"平面")) {
					auto go = m_Scene->CreateGameObject(u8"平面");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Plane));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"球")) {
					auto go = m_Scene->CreateGameObject(u8"球");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Sphere));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"圆锥")) {
					auto go = m_Scene->CreateGameObject(u8"圆锥");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Cone));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"圆柱")) {
					auto go = m_Scene->CreateGameObject(u8"圆柱");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Cylinder));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"胶囊")) {
					auto go = m_Scene->CreateGameObject(u8"胶囊");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Capsule));
					CommandManager::Get().Command_CreateObj(go);
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::MenuItem(u8"点光源")) {
				auto go = m_Scene->CreateGameObject(u8"光源");
				go.AddComponent<PointLightComponent>();
				CommandManager::Get().Command_CreateObj(go);
			}
			if (ImGui::MenuItem(u8"摄像机")) {
				auto go = m_Scene->CreateGameObject(u8"摄像机");
				go.AddComponent<CameraComponent>(CameraType::Perspective);
				CommandManager::Get().Command_CreateObj(go);
			}

				

			ImGui::EndPopup();
		}



		ImGui::End();
	}
	void ScenePanel::DrawGONode(GameObject gameObj)
	{

		ImGuiTreeNodeFlags flags = ((m_Selected == gameObj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObj, flags, gameObj.GetName().c_str());
		// Our buttons are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the index of our item (could be anything)
			ImGui::SetDragDropPayload("DragNodeObj", &gameObj, sizeof(GameObject));
			{ ImGui::Text(u8"移动"); }
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragNodeObj"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject));
				GameObject sub = *(const GameObject*)payload->Data;
				sub.SetParent(&gameObj);				
				
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::IsItemClicked())
		{
			m_Selected = gameObj;

		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(u8"删除对象")) {

				CommandManager::Get().Command_DeleteObj(gameObj);
				entityDeleted = true;
			}
				
			if (ImGui::MenuItem(u8"创建副本")) {
				CommandManager::Get().Command_CreateObj(gameObj.Duplicate());
			}
				
			ImGui::EndPopup();
		}

		if (opened)
		{
			for (entt::entity x : gameObj.GetComponent<TransformComponent>().children) {
				GameObject go(x, m_Scene.get());
				DrawGONode(go);
			}
			ImGui::TreePop();
		}
		if (entityDeleted)
		{
			m_Scene->Destroy(gameObj);
			if (m_Selected == gameObj)
				m_Selected = {};
		}

	}
}