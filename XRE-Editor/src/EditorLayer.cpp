#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include "GLFW\include\GLFW\glfw3.h"
#pragma once
#define PI 3.1415926f
EditorLayer::EditorLayer()
	: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f , glm::vec3(0.0f,2.0f,-5.0f))
{
	
}

void EditorLayer::OnAttach()
{
	m_Framebuffer = Framebuffer::Create(1280, 720);

	//m_NanosuitModel.reset(new Model("./assets/models/nanosuit/nanosuit.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/cube.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/cylinder.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/sphere.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/capsule.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/nanosuit/nanosuit.obj"));
	
	auto defaultShader = ResourceManager::GetShaderLib()->Load("assets/shaders/default.glsl");

	Ref<PointLight> p1= std::make_shared<PointLight>(glm::vec3(1.2f, 2.5f, 0.0f));
	Ref<PointLight> p2 = std::make_shared<PointLight>(glm::vec3(-1.2f, 3.5f, 3.0f));
	Ref<DirectionalLight> dl= std::make_shared<DirectionalLight>(glm::vec3(-0.2f, -1.0f, 0.3f));
	m_Light.AddPLight(p1);
	//m_Light.AddPLight(p2);
	m_Light.SetDirLight(dl);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(XRE::TimeStep ts)
{
	//m_Framebuffer = Framebuffer::Create(m_ViewportSize.x,m_ViewportSize.y);
	m_CameraController.OnUpdate(ts);
	Ref<Camera> camera = m_CameraController.GetCamera();
	m_Framebuffer->Bind();
	// Render
	XRE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	XRE::RenderCommand::Clear();

	XRE::Renderer::BeginScene(m_CameraController.GetCamera());


	m_Light.getPointLight(0)->m_Color = m_SquareColor;
	//m_Light.getPointLight(1)->m_Color = m_color2;
	m_Light.getDirLight()->m_Color = m_color2;

	auto defaultShader = ResourceManager::GetShaderLib()->Get("default");
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->Bind();
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("viewPos", camera->GetPosition());

	m_Light.Draw(defaultShader);
	
	glm::mat4 transform = glm::mat4(1.0f);
	for (int i = 0;i < 4;i++) {
		transform = glm::translate(transform, glm::vec3(2.0f, 0.0f, 0.0f));
		m_Models[i]->Draw(defaultShader, transform);
		
	}
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
	//transform = glm::rotate(transform, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	m_Models[4]->Draw(defaultShader, transform);
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f,0.2f,20.0f));
	transform = glm::translate(transform, glm::vec3(0.0f,-3.0f, 0.0f));
	m_Models[0]->Draw(defaultShader, transform);
	XRE::Renderer::EndScene();
	m_Framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
	
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Exit")) XRE::Application::GetApplication().Close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");

		ImGui::ColorEdit4("PointLight Color", glm::value_ptr(m_SquareColor));
		ImGui::ColorEdit4("DirLight Color", glm::value_ptr(m_color2));

	ImGui::End();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("ViewPort");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			//XRE_CORE_TRACE("viewport: {0} x {1} ", viewportPanelSize.x, viewportPanelSize.y);
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.Resize(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("SceneManager");
	ImGui::End();

	ImGui::Begin("AssetsManager");
	ImGui::End();

	ImGui::End();
		

}

void EditorLayer::OnEvent(XRE::Event& e)
{
	m_CameraController.OnEvent(e);
	
}
