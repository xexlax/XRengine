


project(XRE-CORE)



if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	# msvcpch.cmake的路径
	set(MSVCCMAKE_PCH ${ENGINE_ROOT_DIR})
	# 预编译头文件和源文件的路径
	set(STDAFX_PCH_H ${ENGINE_SRC_DIR}/pch.h)
	set(STDAFX_PCH_C ${ENGINE_SRC_DIR}/pch.cpp)	
	
	# 添加预编译cmake的路径
	LIST(APPEND CMAKE_MODULE_PATH ${MSVCCMAKE_PCH})
	# 包含msvcpch.cmake
	INCLUDE(msvcpch)
endif ()
 
 


set(THIRD_PARTY_INCLUDE  
${THIRD_PARTY_DIR}/spdlog/include
${THIRD_PARTY_DIR}/GLFW/include 
${THIRD_PARTY_DIR}/Glad/include
${THIRD_PARTY_DIR}/ImGui
${THIRD_PARTY_DIR}/glm
${THIRD_PARTY_DIR}/stb_img
${THIRD_PARTY_DIR}/tinyobjloader
${THIRD_PARTY_DIR}/json11
${THIRD_PARTY_DIR}/Entt/single_include
${THIRD_PARTY_DIR}/ImGuizmo
${THIRD_PARTY_DIR}/cereal/include
${THIRD_PARTY_DIR}/imnodes
${THIRD_PARTY_DIR}/openal-soft/include
${THIRD_PARTY_DIR}/OpenXR/include
${THIRD_PARTY_DIR}/VulkanSDK/include
${THIRD_PARTY_DIR}/vma/include
)


file(GLOB_RECURSE HEADER_FILES 
"${ENGINE_SRC_DIR}/*.h"
"${THIRD_PARTY_DIR}/spdlog/include/*.hpp"
"${THIRD_PARTY_DIR}/glm/glm/*.hpp"
"${THIRD_PARTY_DIR}/glm/glm/*.inl"
"${THIRD_PARTY_DIR}/stb_img/stb_image.h"
"${THIRD_PARTY_DIR}/tinyobjloader/tiny_obj_loader.h"
"${THIRD_PARTY_DIR}/json11/*.hpp"
"${THIRD_PARTY_DIR}/Entt/single_include/*.hpp"
"${THIRD_PARTY_DIR}/ImGuizmo/ImGuizmo.h"
"${THIRD_PARTY_DIR}/cereal/include/*.hpp"
"${THIRD_PARTY_DIR}/imnodes/imnodes_internal.h"
"${THIRD_PARTY_DIR}/imnodes/imnodes.h"
"${THIRD_PARTY_DIR}/openal-soft/include/*.h"
"${THIRD_PARTY_DIR}/VulkanSDK/include/vulkan/*.h"
"${THIRD_PARTY_DIR}/vma/include/*.h"

)
file(GLOB_RECURSE SOURCE_FILES 
"${ENGINE_SRC_DIR}/*.cpp"
"${THIRD_PARTY_DIR}/stb_img/stb_image.cpp"
"${THIRD_PARTY_DIR}/ImGuizmo/ImGuizmo.cpp"
"${THIRD_PARTY_DIR}/imnodes/imnodes.cpp"

)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
# Define shared library
# Create nice looking alias for clients of Hazel to link to
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES} ${STDAFX_PCH_C})
USE_MSVC_PCH(${PROJECT_NAME} ${STDAFX_PCH_H} ${STDAFX_PCH_C})
target_compile_definitions(${PROJECT_NAME} PUBLIC XRE_PLATFORM_WINDOWS)
target_compile_definitions(${PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()


target_compile_definitions(${PROJECT_NAME} PUBLIC XRE_RENDERER_OPENGL)
#target_compile_definitions(${PROJECT_NAME} PUBLIC XRE_RENDERER_VULKAN)

set(vulkan_lib "${THIRD_PARTY_DIR}/VulkanSDK/lib/Win32/vulkan-1.lib")
set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32/glslangValidator.exe)
add_compile_definitions("VK_LAYER_PATH=${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32")








# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${THIRD_PARTY_INCLUDE})

# Language standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)



# Link libraries (e.g. logging)
target_link_libraries(${PROJECT_NAME}
        # ${CMAKE_THREAD_LIBS_INIT}     # Needed for unix builds to link to pthread
        # LOGGING
        Glad GLFW ImGui OpenGL32.Lib Jolt OpenAL ${THIRD_PARTY_DIR}/VulkanSDK/lib/Win32/vulkan-1.lib
        )

