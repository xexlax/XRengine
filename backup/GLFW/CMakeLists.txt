project(GLFW)


set(  HEADER_FILES 
"include/GLFW/glfw3.h"
"include/GLFW/glfw3native.h"
"src/context.c"
"src/init.c"
"src/input.c"
"src/monitor.c"

"src/null_init.c"
"src/null_joystick.c"
"src/null_monitor.c"
"src/null_window.c"

"src/platform.c"
"src/vulkan.c"
"src/window.c"
)

set(  WINDOWS_FILES 
            "src/win32_init.c"
			"src/win32_joystick.c"
			"src/win32_module.c"
			"src/win32_monitor.c"
			"src/win32_time.c"
			"src/win32_thread.c"
			"src/win32_window.c"
			"src/wgl_context.c"
			"src/egl_context.c"
			"src/osmesa_context.c"
)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${WINDOWS_FILES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC _GLFW_WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
    target_link_libraries(${PROJECT_NAME} Dwmapi.lib)
endif()




