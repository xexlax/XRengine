#include "pch.h"
#include "ImGuiLayer.h"

#include "xre\Core\application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_vulkan.h"
#include "imnodes.h"


#include "ImGuizmo.h"

// TEMPORARY
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace XRE {

	

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("../Assets/fonts/simhei.ttf",24.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
		/*io.Fonts->AddFontFromFileTTF("../Assets/fonts/opensans/OpenSans-Bold.ttf", 24.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("../Assets/fonts/opensans/OpenSans-Regular.ttf", 24.0f);*/

		// Setup Dear ImGui style
		SetStyle();
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::GetApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings

#ifdef XRE_RENDERER_OPENGL
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
#endif // XRE_RENDERER_OPENGL

#ifdef XRE_RENDERER_VULKAN
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
#endif // XRE_RENDERER_VULKAN

		
		ImNodes::CreateContext();

	}
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImNodes::DestroyContext();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e) {
		if (m_BlockEvents)
		{
			//将event设置为已处理过
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetApplication();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetStyle()
	{
		

		
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.03f, 0.03f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.15f, 0.19f, 0.70f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.36f, 0.41f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.43f, 0.11f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.03f, 0.16f, 0.24f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.32f, 0.11f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.96f, 0.27f, 0.11f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.93f, 0.62f, 0.10f, 0.99f);
		colors[ImGuiCol_Button] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.23f, 0.23f, 0.25f, 0.59f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.03f, 0.13f, 0.18f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.03f, 0.11f, 0.18f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.02f, 0.02f, 0.03f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.08f, 0.30f, 0.46f, 0.59f);
		colors[ImGuiCol_TabActive] = ImVec4(0.02f, 0.11f, 0.18f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.03f, 0.11f, 0.18f, 0.10f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.02f, 0.02f, 0.03f, 0.78f);
		colors[ImGuiCol_DockingPreview] = ImVec4(1.00f, 0.30f, 0.00f, 0.30f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);






	}
	
}