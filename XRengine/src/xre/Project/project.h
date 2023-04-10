#pragma once
#include <cereal\archives\json.hpp>
#include <cereal\types\vector.hpp>

namespace XRE {
	class Project {
	public:
		Project(std::string p);


		std::vector<std::string> m_Scenes;
		std::string m_Name,m_RootPath;




		template <class AR>
		void serialize(AR& ar) {
			ar(m_Name,m_Scenes);
		}

		void Save();

		void Load();
		

	};
}