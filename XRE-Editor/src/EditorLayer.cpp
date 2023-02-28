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
	
	InitScene();
	Renderer3D::Init();
	
}

void EditorLayer::InitScene() {
	//m_NanosuitModel.reset(new Model("./assets/models/nanosuit/nanosuit.obj"));

	CubeGO = m_Scene.CreateGameObject("Cube");
	CylinderGO = m_Scene.CreateGameObject("Cylinder");
	SphereGO = m_Scene.CreateGameObject("Sphere");
	//NanoGO = m_Scene.CreateGameObject("Nanosuit");
	BocchiGO = m_Scene.CreateGameObject("Bocchi");
	FloorGO = m_Scene.CreateGameObject("Floor");

	Ref<Model> cubemesh = std::make_shared<Model>("../Assets/models/cube.obj");
	CubeGO.AddComponent<MeshRendererComponent>(cubemesh);
	FloorGO.AddComponent<MeshRendererComponent>(cubemesh);
	CylinderGO.AddComponent<MeshRendererComponent>(std::make_shared<Model>("../Assets/models/cylinder.obj"));
	SphereGO.AddComponent<MeshRendererComponent>(std::make_shared<Model>("../Assets/models/sphere.obj"));
	//NanoGO.AddComponent<MeshRendererComponent>(std::make_shared<Model>("../Assets/models/nanosuit/nanosuit.obj"));
	BocchiGO.AddComponent<MeshRendererComponent>(std::make_shared<Model>("../Assets/models/bocchi/bocchi.obj"));


	PointLight1GO = m_Scene.CreateGameObject("light1");
	PointLight2GO = m_Scene.CreateGameObject("light2");
	DirLightGO = m_Scene.CreateGameObject("sun");

	PointLight1GO.AddComponent<PointLightComponent>(glm::vec3(0.2f, 0.3f, 0.9f), 4.0f);
	PointLight2GO.AddComponent<PointLightComponent>(glm::vec3(0.8f, 0.4f, 0.2f), 4.0f);
	DirLightGO.AddComponent<DirectionalLightComponent>(glm::vec3(0.5f, 0.4f, 0.35f), 2.0f);

	SceneCameraGO = m_Scene.CreateGameObject("Camera");
	SceneCameraGO.AddComponent<CameraComponent>(Perspective);
	SceneCameraGO.GetComponent<TransformComponent>().m_Translation = glm::vec3(0.0f, 9.0f, 12.0f);
	SceneCameraGO.GetComponent<TransformComponent>().m_Rotation = glm::vec3(45.0f, 0.0f, 0.0f);

	BocchiGO.GetComponent<TransformComponent>().m_Translation = glm::vec3(-5.0f, 0.0f, 2.0f);
	BocchiGO.GetComponent<TransformComponent>().m_Rotation = glm::vec3(0.0f, 180.0f, 0.0f);

	FloorGO.GetComponent<TransformComponent>().m_Translation = glm::vec3(0.0f, -1.0f, 0.0f);
	FloorGO.GetComponent<TransformComponent>().m_Scale = glm::vec3(20.0f, 0.2f, 20.0f);

	DirLightGO.GetComponent<TransformComponent>().m_Rotation = glm::vec3(45.0f, -45.0f, 0.0f);

	Renderer3D::m_Light.SetDirLight(DirLightGO);

	
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
	//Temp：更新场景属性
	SetScene();
	//场景处理
	m_Scene.OnUpdate(ts);

}

void EditorLayer::SetScene()
{
	
	

	float t = ImGui::GetTime();


	PointLight1GO.GetComponent<TransformComponent>().m_Translation =
		 glm::vec3(2 * cos(2 * t), 2.5f, 2 * sin(2 * t));

	PointLight2GO.GetComponent<TransformComponent>().m_Translation =
		glm::vec3(2 * cos(2 * t + PI), 2.5f, 2 * sin(2 * t + PI));

	Renderer3D::m_Light.SetPLight(PointLight1GO, 0);
	Renderer3D::m_Light.SetPLight(PointLight2GO, 1);
	Renderer3D::m_Light.SetDirLight(DirLightGO);

	int i = 0;
	
	CubeGO.GetComponent<TransformComponent>().m_Translation = glm::vec3(1.3 * (i + 1) * cos(t / (i + 1)), 0, 1.3 * (i + 1) * sin(t / (i + 1)));
	CubeGO.GetComponent<TransformComponent>().m_Rotation = glm::vec3(t*10);
	i = 1;
	CylinderGO.GetComponent<TransformComponent>().m_Translation = glm::vec3(1.3 * (i + 1) * cos(t / (i + 1)), 0, 1.3 * (i + 1) * sin(t / (i + 1)));
	CylinderGO.GetComponent<TransformComponent>().m_Rotation = glm::vec3(t * 10);
	i = 2;
	SphereGO.GetComponent<TransformComponent>().m_Translation = glm::vec3(1.3 * (i + 1) * cos(t / (i + 1)), 0, 1.3 * (i + 1) * sin(t / (i + 1)));
	SphereGO.GetComponent<TransformComponent>().m_Rotation = glm::vec3(t * 10);
	
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

		if (ImGui::Button("ReloadShader")) {
			Renderer3D::Init();
		}
		ImGui::DragFloat3("Camera Translation",
			glm::value_ptr(SceneCameraGO.GetComponent<TransformComponent>().m_Translation),0.1f);

		ImGui::DragFloat3("Camera Rotation",
			glm::value_ptr(SceneCameraGO.GetComponent<TransformComponent>().m_Rotation),1.0f);

		ImGui::DragFloat3("Sun Direction",
			glm::value_ptr(DirLightGO.GetComponent<TransformComponent>().m_Rotation), 1.0f);

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
			m_Scene.OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
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



