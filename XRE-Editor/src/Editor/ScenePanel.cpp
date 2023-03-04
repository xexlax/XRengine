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
	}
	void ScenePanel::Select(int ObjId)
	{
		GameObject go( entt::entity(ObjId) , m_Scene.get() );
		m_Selected = go;
	}
	void ScenePanel::OnImGuiRender()
	{
		ImGui::Begin(u8"场景");

		m_Scene->m_Registry.each([&](auto entityID)
			{
				//shared_ptr.get 返回其raw pointer
				GameObject go{ entityID , m_Scene.get() };
				DrawGONode(go);
			});


		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_Selected = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem(u8"新的对象"))
				m_Scene->CreateGameObject(u8"空对象");

			ImGui::EndPopup();
		}



		ImGui::End();
	}
	void ScenePanel::DrawGONode(GameObject gameObj)
	{

		ImGuiTreeNodeFlags flags = ((m_Selected == gameObj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObj, flags, gameObj.GetName().c_str());
		if (ImGui::IsItemClicked())
		{
			m_Selected = gameObj;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(u8"删除对象"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			
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