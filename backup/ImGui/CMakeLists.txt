project(ImGui)


set(HEADER_FILES 
    "imconfig.h"
    "imgui.h"
    "imgui.cpp"
    "imgui_draw.cpp"
    "imgui_internal.h"
	"imgui_tables.cpp"
	"imgui_widgets.cpp"
	"imstb_rectpack.h"
	"imstb_textedit.h"
	"imstb_truetype.h"
	"imgui_tables.cpp"
	"imgui_demo.cpp"
)


add_library(${PROJECT_NAME} STATIC ${HEADER_FILES})
