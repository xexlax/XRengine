#include "MaterialPanel.h"
#include <ImGui\imgui.h>
#include "Data.h"
#include <glm\glm\gtc\type_ptr.hpp>
namespace XRE {
	MaterialPanel::MaterialPanel()
	{
	}
	void MaterialPanel::OnImGuiRender()
	{
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
				uint32_t tex;
				if (m_Selected->diffuseTex.m_enable) {
					tex = m_Selected->diffuseTex.m_Tex->GetRendererId();
					if (ImGui::Button(u8"delete")) {
						m_Selected->diffuseTex.m_enable = false;
					}
				}
				else tex = ResourceManager::GetNoTex2D()->GetRendererId();

				ImGui::Image((void*)tex, ImVec2{ 200, 200 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
				if (ImGui::BeginDragDropTarget()) {


					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
					{
						std::string path = *(std::string*)payload->Data;

						if (path.find(".png") != string::npos || path.find(".jpg") != string::npos) {
							m_Selected->diffuseTex.m_enable = true;
							m_Selected->diffuseTex.m_filepath = path;
							m_Selected->diffuseTex.m_name = Utils::GetFileName(path);
							m_Selected->diffuseTex.m_Tex = ResourceManager::GetTex2D(path);
						}

					}
					ImGui::EndDragDropTarget();
				}
			}
			ImGui::Text("SpecularMap");
			{
				uint32_t tex;
				if (m_Selected->specularTex.m_enable) {
					tex = m_Selected->specularTex.m_Tex->GetRendererId();
					if (ImGui::Button(u8"delete")) {
						m_Selected->specularTex.m_enable = false;
					}
				}
				else tex = ResourceManager::GetNoTex2D()->GetRendererId();

				ImGui::Image((void*)tex, ImVec2{ 200, 200 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
				if (ImGui::BeginDragDropTarget()) {


					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
					{
						std::string path = *(std::string*)payload->Data;

						if (path.find(".png") != string::npos || path.find(".jpg") != string::npos) {

							m_Selected->specularTex.m_enable = true;
							m_Selected->specularTex.m_filepath = path;
							m_Selected->specularTex.m_name = Utils::GetFileName(path);
							m_Selected->specularTex.m_Tex = ResourceManager::GetTex2D(path);
						}

					}
					ImGui::EndDragDropTarget();
				}
			}
			ImGui::Text("NormalMap");
			{
				uint32_t tex;
				if (m_Selected->bumpTex.m_enable) {
					tex = m_Selected->bumpTex.m_Tex->GetRendererId();
					if (ImGui::Button(u8"delete")) {
						m_Selected->bumpTex.m_enable = false;
					}
				}
				else tex = ResourceManager::GetNoTex2D()->GetRendererId();

				ImGui::Image((void*)tex, ImVec2{ 200, 200 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
				if (ImGui::BeginDragDropTarget()) {


					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
					{
						std::string path = *(std::string*)payload->Data;

						if (path.find(".png") != string::npos || path.find(".jpg") != string::npos) {

							m_Selected->bumpTex.m_enable = true;
							m_Selected->bumpTex.m_filepath = path;
							m_Selected->bumpTex.m_name = Utils::GetFileName(path);
							m_Selected->bumpTex.m_Tex = ResourceManager::GetTex2D(path);
						}

					}
					ImGui::EndDragDropTarget();
				}
			}
			
		}
		

		ImGui::End();
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
	}
}