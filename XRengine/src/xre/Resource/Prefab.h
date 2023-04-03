#pragma once
#include "xre\ECS\GameObject.h"

namespace XRE {

	class Prefab {
	public:
		Prefab() = default;
		Prefab(std::string s) :m_FilePath(s) {};
		static void Save(GameObject prototype, std::string filepath);

		
		std::string m_FilePath;
	};
}