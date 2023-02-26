#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"


//temperory


#pragma once
#define PI 3.1415926f

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
EditorLayer::EditorLayer()
	: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f , glm::vec3(0.0f,2.0f,5.0f))
{
	
}

void EditorLayer::OnAttach()
{
	

	//m_NanosuitModel.reset(new Model("./assets/models/nanosuit/nanosuit.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/cube.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/cylinder.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/sphere.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/capsule.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/nanosuit/nanosuit.obj"));

	//学了三年建模系列
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/bocchi/bocchi.obj"));

	Ref<PointLight> p1= std::make_shared<PointLight>(glm::vec3(1.2f, 2.5f, 0.0f));

	Ref<PointLight> p2 = std::make_shared<PointLight>(glm::vec3(-1.2f, 2.5f, 3.0f));
	Ref<DirectionalLight> dl= std::make_shared<DirectionalLight>(glm::vec3(4.0f, -4.0f, 4.0f));
	m_Light.AddPLight(p1);
	m_Light.AddPLight(p2);
	m_Light.SetDirLight(dl);


	Renderer3D::Init();

	
}

void EditorLayer::OnDetach()
{
	Renderer3D::ShutDown();
}

void EditorLayer::OnUpdate(XRE::TimeStep ts)
{
	
	if (m_ViewportFocused) {
		m_CameraController.OnUpdate(ts);
		//Application::GetApplication().GetWindow().HideCursor(m_ViewportFocused);
	}
		

	Ref<Camera> camera = m_CameraController.GetCamera();

	m_Light.getPointLight(0)->m_Color = m_SquareColor;
	m_Light.getPointLight(1)->m_Color = m_color3;
	m_Light.getDirLight()->m_Color = m_color2;
	m_Light.getDirLight()->m_Intensity = m_DirLightIntensity;
	float t = ImGui::GetTime();

	m_Light.getPointLight(0)->m_Position = glm::vec3(2 * cos(2 * t), 2.5f, 2 * sin(2 * t));
	m_Light.getPointLight(1)->m_Position = glm::vec3(2 * cos(2 * t + PI), 2.5f, 2 * sin(2 * t + PI));




	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });


	//ShadowMap Pass
	if (m_Show_Shadow) {
		Renderer3D::StartShadowPass(m_Light.getDirLight());
		DrawScene();
		Renderer3D::EndShadowPass();
	}
	

	// Coloring Pass

	Renderer3D::m_FrameBuffer->Bind();
	RenderCommand::Clear();

	if (m_PBR) Renderer3D::activeShader = Renderer3D::defaultPBRShader;
	else Renderer3D::activeShader = Renderer3D::defaultObjShader;

	//切shader一定要在startscene前
	Renderer3D::StartScene(camera);
	{
		Renderer3D::activeShader->Bind();
		Renderer3D::activeShader->SetFloat("material.metallic", m_Metallic);
		Renderer3D::activeShader->SetFloat("material.roughness", m_Roughness);
		if(m_Show_Shadow)
		Renderer3D::SetShadowMapOfActive(0);
		Renderer3D::DrawLight(make_shared<Light>(m_Light));

		DrawScene();
		if (m_ShowSkybox)
			Renderer3D::DrawSkybox();
	}
	Renderer3D::EndScene();
	Renderer3D::m_FrameBuffer->Unbind();

	//Renderer3D::PostProcessing();

}
void EditorLayer::DrawScene()
{
	
	

	float t = ImGui::GetTime();
	glm::mat4 transform = glm::mat4(1.0f);
	//Renderer3D::DrawModel(m_Models[0], transform);
	transform = glm::rotate(transform, PI, glm::vec3(0, 1, 0));
	transform = glm::scale(transform, glm::vec3(0.5f));
	transform = glm::translate(transform, glm::vec3(-5.0f, 0.0f, 2.0f));

	Renderer3D::DrawModel(m_Models[5], transform);
	for (int i = 0;i < 4;i++) {
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.3 * (i + 1) * cos(t / (i + 1)), 0, 1.3 * (i + 1) * sin(t / (i + 1))));
		transform = glm::rotate(transform, t, glm::vec3(1, 1, 1));
		Renderer3D::DrawModel(m_Models[i], transform);

	}


	transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
	transform = glm::translate(transform, glm::vec3(0.0f, -2.5f, -2.0f));
	Renderer3D::DrawModel(m_Models[4], transform);


	transform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 0.2f, 20.0f));
	transform = glm::translate(transform, glm::vec3(0.0f, -3.0f, 0.0f));
	Renderer3D::DrawModel(m_Models[0], transform);
}

void EditorLayer::OnImGuiRender(){
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
			if (ImGui::MenuItem("Open"));
			if (ImGui::MenuItem("Save"));
			if (ImGui::MenuItem("Exit")) XRE::Application::GetApplication().Close();
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"));
		if (ImGui::BeginMenu("Windows"));
		if (ImGui::BeginMenu("Help"));
		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");
		ImGui::Text("FPS:%d", Application::GetFPS());
		ImGui::Separator();

		ImGui::ColorEdit4("PointLight1 Color", glm::value_ptr(m_SquareColor));
		ImGui::ColorEdit4("PointLight2 Color", glm::value_ptr(m_color3));
		ImGui::ColorEdit4("DirLight Color", glm::value_ptr(m_color2));
		//ImGui::InputFloat("DirLight Intensity", &m_DirLightIntensity);
		ImGui::SliderFloat("DirLight Intensity", &m_DirLightIntensity, 0.0f, 5.0f);
		ImGui::Separator();

		//ImGui::Checkbox("Shadow", &m_Show_Shadow );
		ImGui::Checkbox("PBR Shading", &m_PBR);
		if (m_PBR) {
			ImGui::SliderFloat("Metallic", &m_Metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
		}

		if (ImGui::Button("ReloadShader")) {
			Renderer3D::Init();
		}

		


		ImGui::Checkbox("SkyBox", &m_ShowSkybox);
		uint32_t mapID = Renderer3D::m_ShadowFrameBuffer->GetDepthAttachment();

		ImGui::Separator();
		ImGui::Text("DepthMap");
		ImGui::Image((void*)mapID, ImVec2{ 300, 300 }, ImVec2{ 0,1 }, ImVec2{ 1,0 }) ;

	ImGui::End();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("ViewPort");
		m_ViewportFocused = ImGui::IsWindowFocused();
		
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::GetApplication().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);


		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			//XRE_CORE_TRACE("viewport: {0} x {1} ", viewportPanelSize.x, viewportPanelSize.y);
			Renderer3D::m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.Resize(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureID = Renderer3D::m_FrameBuffer->GetColorAttachment();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("SceneManager");
	ImGui::End();

	ImGui::Begin("AssetManager");
	ImGui::End();

	//ImGui::ShowDemoWindow();

	ImGui::End();
		

}



void EditorLayer::OnEvent(XRE::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyReleased));
	m_CameraController.OnEvent(e);
	
}

bool EditorLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	if (e.GetKeyCode() == XRE_KEY_ESCAPE) {
		ImGui::SetWindowFocus(false);
	}
	return false;
}



