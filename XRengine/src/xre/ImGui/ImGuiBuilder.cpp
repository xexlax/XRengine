
#include "pch.h"
#ifdef XRE_RENDERER_OPENGL
	#define  IMGUI_IMPL_OPENGL_LOADER_GLAD
	#include "backends\imgui_impl_opengl3.cpp"
#endif

#ifdef XRE_RENDERER_VULKAN
	#include "backends\imgui_impl_vulkan.cpp"

#endif
#include "backends\imgui_impl_glfw.cpp"
