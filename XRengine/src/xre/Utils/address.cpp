#include "address.h"

namespace XRE::Utils {
	std::string GetFileName(const std::string& path) {
		std::string name;
		size_t pos = path.find_last_of('\\');
		if (pos != std::string::npos) {
			name = path.substr(pos + 1);
		}

		return name;
	}

	std::string GetBasePath(const std::string& path) {
		std::string basepath;
		size_t pos = path.find_last_of('\\');
		if (pos != std::string::npos) {
			basepath = path.substr(0, pos);
		}
		return basepath;
	}
}