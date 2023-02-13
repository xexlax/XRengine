#pragma once
#ifdef XRE_BUILD_DLL
	#define DECL __declspec(dllexport)
#else
	#define DECL __declspec(dllimport)
#endif // XRE_BUILD_DLL
