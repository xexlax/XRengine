#include "MaterialPanel.h"
#include "PanelsManager.h"
#include <ImGui\imgui.h>
#include "imnodes\imnodes.h"
#include "Data.h"
#include <glm\glm\gtc\type_ptr.hpp>
namespace XRE {
	MaterialPanel::MaterialPanel()
	{
	}
	void MaterialPanel::OnImGuiRender()
	{
		ImGui::Begin("node editor");
		const int hardcoded_node_id = 1;

		ImNodes::BeginNodeEditor();

		ImNodes::BeginNode(hardcoded_node_id);
		ImGui::Dummy(ImVec2(80.0f, 45.0f));
		ImNodes::EndNode();

		ImNodes::BeginNode(hardcoded_node_id);

		const int output_attr_id = 2;
		ImNodes::BeginOutputAttribute(output_attr_id);
		// in between Begin|EndAttribute calls, you can call ImGui
		// UI functions
		ImGui::Text("output pin");
		ImNodes::EndOutputAttribute();

		ImNodes::EndNode();

		ImNodes::EndNodeEditor();

		ImGui::End();

		ImGui::Begin(u8"材质");
		if (m_Switch) {
			ImGui::SetWindowFocus();
			m_Switch = false;
		}
		
		if (m_Selected != nullptr) {

			XUI::InputText(u8"地址", &m_Selected->GetPath());
			if(ImGui::Button(u8"保存") ){
				m_Selected->Save();
			}

			ImGui::ColorEdit3(u8"基础色", glm::value_ptr(m_Selected->baseColor));
			XUI::DragFloat("Shininess", &m_Selected->shininess);
			XUI::DragFloat("Metallic", &m_Selected->metallic);
			XUI::DragFloat("Roughness", &m_Selected->roughness);
			
			ImGui::Text("Albedo");
			{
				DrawTextureSlot(&m_Selected->diffuseTex);
			}
			ImGui::Text("SpecularMap");
			{
				DrawTextureSlot(&m_Selected->specularTex);
			}
			ImGui::Text("NormalMap");
			{
				DrawTextureSlot(&m_Selected->diffuseTex);
			}
			
		}
		

		ImGui::End();
	}
	void MaterialPanel::DrawTextureSlot(MaterialTex* MatTex)
	{
		uint32_t tex;
		if (MatTex->m_enable) {
			tex = MatTex->m_Tex->GetRendererId();
			if (ImGui::Button(u8"delete")) {
				MatTex->m_enable = false;
			}
		}
		else tex = ResourceManager::GetNoTex2D()->GetRendererId();

		ImGui::Image((void*)tex, ImVec2{ 200, 200 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		if (ImGui::BeginDragDropTarget()) {


			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
			{
				std::string path = *(std::string*)payload->Data;

				if (path.find(".png") != string::npos || path.find(".jpg") != string::npos) {
					MatTex->m_enable = true;
					MatTex->m_filepath = path;
					MatTex->m_name = Utils::GetFileName(path);
					MatTex->m_Tex = ResourceManager::GetTex2D(path);
				}

			}
			ImGui::EndDragDropTarget();
		}
	}
	void MaterialPanel::Select(XRef<Material> s,MeshRendererComponent *c)
	{
		m_Selected = s;
		comp = c;
		Switch();
	}
	void MaterialPanel::Switch()
	{
		m_Switch = true;

		if (PanelsManager::GetScenePanel()->GetSelected().HasComponent<MeshRendererComponent>()) {
			m_Selected =
			PanelsManager::GetScenePanel()->GetSelected().GetComponent<MeshRendererComponent>().GetMaterial();
		}
	}
}