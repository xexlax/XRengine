#include "ScenePanel.h"

#include <ImGui\imgui.h>
namespace XRE{
	ScenePanel::ScenePanel(const Ref<Scene>& scene)
	{
		AttachToScene(scene);
	}
	void ScenePanel::AttachToScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_Selected = GameObject(entt::null ,m_Scene.get());
	}
	void ScenePanel::Select(int ObjId)
	{
		GameObject go( entt::entity(ObjId) , m_Scene.get() );
		m_Selected = go;
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
					m_Scene->CreateGameObject(u8"������").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Cube)
						);

				}
				if (ImGui::MenuItem(u8"ƽ��")) {
					m_Scene->CreateGameObject(u8"ƽ��").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Plane)
						);
				}
				if (ImGui::MenuItem(u8"��")) {
					m_Scene->CreateGameObject(u8"��").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Sphere)
						);
				}
				if (ImGui::MenuItem(u8"Բ׶")) {
					m_Scene->CreateGameObject(u8"Բ׶").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Cone)
						);
				}
				if (ImGui::MenuItem(u8"Բ��")) {
					m_Scene->CreateGameObject(u8"Բ��").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Cylinder)
						);
				}
				if (ImGui::MenuItem(u8"Monkey")) {
					m_Scene->CreateGameObject(u8"Monkey").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Torus)
						);
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::MenuItem(u8"���Դ")) {
				m_Scene->CreateGameObject(u8"��Դ").AddComponent<PointLightComponent>();
				
			}
			if (ImGui::MenuItem(u8"�����")) {
				m_Scene->CreateGameObject(u8"�����").AddComponent<CameraComponent>();
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
			if (ImGui::MenuItem(u8"ɾ������"))
				entityDeleted = true;
			if (ImGui::MenuItem(u8"��������"))
				gameObj.Duplicate();
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