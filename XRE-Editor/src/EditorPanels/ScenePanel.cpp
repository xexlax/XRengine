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
		ImGui::Begin(u8"����");
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
					//shared_ptr.get ������raw pointer
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
			if (ImGui::MenuItem(u8"�ն���")) {
				m_Scene->CreateGameObject(u8"�ն���");
			}

			if (ImGui::BeginMenu(u8"Ԥ����"))
			{
				if (ImGui::MenuItem(u8"������")) {

					auto go = m_Scene->CreateGameObject(u8"������");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Cube));
					CommandManager::Get().Command_CreateObj(go);


				}
				if (ImGui::MenuItem(u8"ƽ��")) {
					auto go = m_Scene->CreateGameObject(u8"ƽ��");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Plane));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"��")) {
					auto go = m_Scene->CreateGameObject(u8"��");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Sphere));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"Բ׶")) {
					auto go = m_Scene->CreateGameObject(u8"Բ׶");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Cone));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"Բ��")) {
					auto go = m_Scene->CreateGameObject(u8"Բ��");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Cylinder));
					CommandManager::Get().Command_CreateObj(go);
				}
				if (ImGui::MenuItem(u8"����")) {
					auto go = m_Scene->CreateGameObject(u8"����");
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetElementalModel(Elemental_Model::Capsule));
					CommandManager::Get().Command_CreateObj(go);
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::MenuItem(u8"���Դ")) {
				auto go = m_Scene->CreateGameObject(u8"��Դ");
				go.AddComponent<PointLightComponent>();
				CommandManager::Get().Command_CreateObj(go);
			}
			if (ImGui::MenuItem(u8"�����")) {
				auto go = m_Scene->CreateGameObject(u8"�����");
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
			{ ImGui::Text(u8"�ƶ�"); }
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
			if (ImGui::MenuItem(u8"ɾ������")) {

				CommandManager::Get().Command_DeleteObj(gameObj);
				entityDeleted = true;
			}
				
			if (ImGui::MenuItem(u8"��������")) {
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