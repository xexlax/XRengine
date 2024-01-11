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
		//DrawNodeEditor();
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
			DrawTextureSlot(&m_Selected->diffuseTex);
			ImGui::Text("SpecularMap");
			DrawTextureSlot(&m_Selected->specularTex);
			ImGui::Text("NormalMap");
			DrawTextureSlot(&m_Selected->bumpTex);
			
		}
		

		ImGui::End();
	}
	void MaterialPanel::DrawNodeEditor()
	{

		ImGui::Begin("node editor");
		const int hardcoded_node_id = 1;
		static glm::vec4 color;
		static std::vector<std::pair<int, int>> links;
		ImNodes::BeginNodeEditor();
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

		ImNodes::BeginNode(10);
		ImNodes::BeginNodeTitleBar();
		ImGui::TextUnformatted(u8"Pure Color");
		ImNodes::EndNodeTitleBar();
		ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

		ImNodes::BeginOutputAttribute(11);
		ImGui::Text(u8"OutPut");
		ImNodes::EndOutputAttribute();

		ImNodes::EndNode();

		ImNodes::BeginNode(hardcoded_node_id);
		ImNodes::BeginNodeTitleBar();
		ImGui::TextUnformatted(u8"输出");
		ImNodes::EndNodeTitleBar();


		ImNodes::BeginInputAttribute(5);
		ImGui::Text(u8"BaseColor");

		ImNodes::EndInputAttribute();

		ImNodes::BeginInputAttribute(2);
		ImGui::Text(u8"Albedo");
		ImNodes::EndInputAttribute();

		ImNodes::BeginInputAttribute(3);
		ImGui::Text(u8"Specular");
		ImNodes::EndInputAttribute();

		ImNodes::BeginInputAttribute(4);
		ImGui::Text(u8"Bump");
		ImNodes::EndInputAttribute();

		ImNodes::EndNode();


		// elsewhere in the code...
		for (int i = 0; i < links.size(); ++i)
		{
			const std::pair<int, int> p = links[i];
			// in this case, we just use the array index of the link
			// as the unique identifier
			
			ImNodes::Link(i, p.first, p.second);
			

		}

		ImNodes::EndNodeEditor();
		int start_attr, end_attr;
		if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
		{

			links.push_back(std::make_pair(start_attr, end_attr));
		}
		int i;
		if (ImNodes::IsLinkDestroyed(&i)) {
			links.erase(links.begin() + i);
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
		m_Switch = true;
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