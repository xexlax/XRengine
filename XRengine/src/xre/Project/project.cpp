#include "xre\Utils\address.h"
#include "project.h"

namespace XRE {
	Project::Project(std::string p):
		m_RootPath(Utils::GetBasePath(p)),m_Name(Utils::GetPureFileName(p))
	{
	}

	void Project::Save()
	{
		std::ofstream fs(m_RootPath+"\\" +m_Name+  ".xreprj");

		cereal::JSONOutputArchive ar(fs);
		ar(*this);
		
	}

	void Project::Load()
	{
		std::ifstream fs(m_RootPath + "\\" + m_Name + ".xreprj");

		cereal::JSONInputArchive ar(fs);

		ar(*this);
	}
}