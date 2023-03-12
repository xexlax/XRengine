#pragma once

#include <filesystem>

namespace XRE {

	class AssetsPanel
	{
	public:
		AssetsPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
	};

}