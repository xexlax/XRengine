#pragma once

#include <filesystem>

#include "xre\Renderer\Resources\Texture.h"
namespace XRE {

	class AssetsPanel
	{
	public:
		AssetsPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		std::string m_DragSource;

		XRef<Texture2D> m_DirectoryIcon, m_FileIcon, m_ModelIcon, m_SceneIcon,m_ShaderIcon ,m_TexIcon;
	};

}