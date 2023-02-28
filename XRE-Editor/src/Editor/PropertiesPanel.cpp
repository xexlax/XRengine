#include <ImGui\imgui.h>
#include "PropertiesPanel.h"
#include <glm\glm\gtc\type_ptr.hpp>

namespace XRE {
	PropertiesPanel::PropertiesPanel(ScenePanel* sp)
	{
		SetReference(sp);
	}
	void PropertiesPanel::SetReference(ScenePanel* sp)
	{
		m_ReferenceScenePanel = sp;
	}
	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");

		GameObject go = m_ReferenceScenePanel->m_Selected;
		if (go) {
			if (go.HasComponent<NameComponent>())
			{
				auto& tag = go.GetComponent<NameComponent>().m_ObjName;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("Name", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			}

			if (go.HasComponent<TransformComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
				{
					auto& transform = go.GetComponent<TransformComponent>();
					ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Translation), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 1.0f);
					ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.1f);
					ImGui::TreePop();
				}
			}
		}

		ImGui::End();
		


	}
	
}