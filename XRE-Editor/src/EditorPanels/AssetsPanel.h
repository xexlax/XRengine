#pragma once

#include <filesystem>

#include "xre\Renderer\Resources\Texture.h"
namespace XRE {

	class AssetsPanel
	{
	public:
		AssetsPanel();
		void SetRootDir(std::string assetpath);
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		std::string m_DragSource;
		std::string m_Assets;

		XRef<Texture2D> 
			m_DirectoryIcon, m_FileIcon, m_ModelIcon,m_BluePrintIcon, m_GOIcon, 
			m_AudioIcon, m_SceneIcon,m_ShaderIcon ,m_TexIcon, m_MaterialIcon;
	};

}