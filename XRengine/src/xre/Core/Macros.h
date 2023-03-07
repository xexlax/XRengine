#pragma once

#ifdef XRE_DYNAMIC_LINK
	#ifdef XRE_BUILD_DLL
		#define DECL __declspec(dllexport)
	#else
		#define DECL __declspec(dllimport)
	#endif // XRE_BUILD_DLL
#else 
	#define DECL 
#endif


#ifdef XRE_ENABLE_ASSERTS
#define XRE_ASSERT(x, ...) { if(!(x)) { XRE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define XRE_CORE_ASSERT(x, ...) { if(!(x)) { XRE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define XRE_ASSERT(x, ...)
#define XRE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) ( 1 << x )
//包装成员函数为 std::function的类型
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#include <memory>
namespace XRE{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using XRef = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr XRef<T> XMakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}