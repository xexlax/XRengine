#include "pch.h"
#include "Prefab.h"
#include <cereal\archives\json.hpp>

namespace XRE {
	void Prefab::Save(GameObject prototype, std::string filepath)
	{
		ofstream fs(filepath);
		cereal::JSONOutputArchive ar(fs);
		ar(prototype);
	}

}