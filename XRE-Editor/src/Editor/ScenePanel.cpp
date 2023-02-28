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
	void ScenePanel::OnImGuiRender()
	{
		ImGui::Begin("Scene");

		m_Scene->m_Registry.each([&](auto entityID)
			{
				//shared_ptr.get ·µ»ØÆäraw pointer
				GameObject go{ entityID , m_Scene.get() };
				DrawGONode(go);
			});

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

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObj, flags, gameObj.GetName().c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}
}