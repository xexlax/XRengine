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
					m_Scene->CreateGameObject(u8"立方体").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Cube)
						);

				}
				if (ImGui::MenuItem(u8"平面")) {
					m_Scene->CreateGameObject(u8"平面").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Plane)
						);
				}
				if (ImGui::MenuItem(u8"球")) {
					m_Scene->CreateGameObject(u8"球").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Sphere)
						);
				}
				if (ImGui::MenuItem(u8"圆锥")) {
					m_Scene->CreateGameObject(u8"圆锥").AddComponent<MeshRendererComponent>(
						ResourceManager::GetElementalModel(Elemental_Model::Cone)
						);
				}
				if (ImGui::MenuItem(u8"圆柱")) {
					m_Scene->CreateGameObject(u8"圆柱").AddComponent<MeshRendererComponent>(
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
				
			if (ImGui::MenuItem(u8"点光源")) {
				m_Scene->CreateGameObject(u8"光源").AddComponent<PointLightComponent>();
				
			}
			if (ImGui::MenuItem(u8"摄像机")) {
				m_Scene->CreateGameObject(u8"摄像机").AddComponent<CameraComponent>();
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
			if (ImGui::MenuItem(u8"删除对象"))
				entityDeleted = true;
			if (ImGui::MenuItem(u8"创建副本"))
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