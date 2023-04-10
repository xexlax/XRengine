#pragma once
#include <string>

namespace XRE {

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string OpenFile(const char* filter);
		static void mCopyFile(const char* src, const char* dest);
		static std::string SaveFile(const char* filter);
	};

}