workspace "XRengine"
	architecture "x64"
	startproject "Sandbox"
	

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "XRengine/3rdparty/GLFW/include"
IncludeDir["Glad"] = "XRengine/3rdparty/Glad/include"
IncludeDir["ImGui"] = "XRengine/3rdparty/ImGui"
IncludeDir["glm"] = "XRengine/3rdparty/glm"


include "XRengine/3rdparty/GLFW"
include "XRengine/3rdparty/Glad"
include "XRengine/3rdparty/ImGui"



project "XRengine"
	location "XRengine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "XRengine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/3rdparty/glm/glm/**.hpp",
		"%{prj.name}/3rdparty/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/3rdparty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		
		systemversion "latest"

		defines
		{
			"XRE_PLATFORM_WINDOWS",
			"XRE_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

		

	filter "configurations:Debug"
		defines "XRE_DEBUG"
        runtime "Debug"
		symbols "on"
        

	filter "configurations:Release"
		defines "XRE_RELEASE"
        runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "XRE_DIST"
        runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"XRengine/3rdparty/spdlog/include",
		"XRengine/src",
		"%{IncludeDir.glm}",
		"XRengine/3rdparty"
	}

	links
	{
		"XRengine"
	}

	filter "system:windows"
		
		systemversion "latest"

		defines
		{
			"XRE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "XRE_DEBUG"
        runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "XRE_RELEASE"
        runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "XRE_DIST"
        runtime "Release"
		optimize "on"