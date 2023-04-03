#include "pch.h"
#include "AssetsPanel.h"
#include <XRE.h>
#include <imgui/imgui.h>

namespace XRE {

	// Once we have projects, change this
	static const std::filesystem::path s_AssetPath = "../Assets";

	AssetsPanel::AssetsPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = ResourceManager::GetTex2D("../Assets/textures/dir_logo.png");
		m_FileIcon = ResourceManager::GetTex2D("../Assets/textures/file_logo.png");
		m_ModelIcon = ResourceManager::GetTex2D("../Assets/textures/model_logo.png");
		m_SceneIcon = ResourceManager::GetTex2D("../Assets/textures/scene_logo.png");
		m_ShaderIcon = ResourceManager::GetTex2D("../Assets/textures/shader_logo.png");
		m_TexIcon = ResourceManager::GetTex2D("../Assets/textures/tex_logo.png");
		m_MaterialIcon = ResourceManager::GetTex2D("../Assets/textures/mat_logo.png");
		m_BluePrintIcon = ResourceManager::GetTex2D("../Assets/textures/bp_logo.png");
	}

	void AssetsPanel::OnImGuiRender()
	{
		ImGui::Begin(u8"资产管理器");
	
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragNodeObj"))
			{
				GameObject go = *(GameObject*)payload->Data;
				Prefab::Save(go, m_CurrentDirectory.string()+ "\\" + go.GetName() + ".go");
			}
			ImGui::EndDragDropTarget();
		}

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button(u8"返回"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		


		static float padding = 2.0f;
		static float thumbnailSize = 160.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
		uint32_t iconid= m_FileIcon->GetRendererId();
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();
			if (directoryEntry.is_directory())

			if(directoryEntry.is_directory()) iconid =m_DirectoryIcon->GetRendererId();
			if (filenameString.find(".obj") !=string::npos) iconid = m_ModelIcon->GetRendererId();
			if (filenameString.find(".scene") != string::npos) iconid = m_SceneIcon->GetRendererId();
			if (filenameString.find(".glsl") != string::npos) iconid = m_ShaderIcon->GetRendererId();
			if (filenameString.find(".png") != string::npos || filenameString.find(".jpg") != string::npos) iconid = m_TexIcon->GetRendererId();
			if (filenameString.find(".mat") != string::npos) iconid = m_MaterialIcon->GetRendererId();
			if (filenameString.find(".bp") != string::npos) iconid = m_BluePrintIcon->GetRendererId();
			ImGui::PushID(filenameString.c_str());
			
			ImGui::ImageButton(ImTextureID(iconid), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			
			if (ImGui::BeginDragDropSource())
			{
				m_DragSource = (m_CurrentDirectory / path.filename()).string();
				ImGui::SetDragDropPayload("AssetItem", &m_DragSource, sizeof(m_DragSource));
				ImGui::EndDragDropSource();
			}

			
			
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{

					if (directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();
								
				
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();

		}

		ImGui::Columns(1);

		
		
		ImGui::End();
	}

}