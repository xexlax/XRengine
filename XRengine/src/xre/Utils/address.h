#pragma once

#include <string>
namespace XRE::Utils {
	std::string GetFileName(const std::string& path);
	std::string GetPureFileName(const std::string& path);
	std::string GetBasePath(const std::string& path);
	std::string GetRidOfRoot(const std::string& path, const std::string& root);
}