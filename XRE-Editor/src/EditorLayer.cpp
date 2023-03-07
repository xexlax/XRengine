#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



//temperory


#pragma once
#define PI 3.1415926f

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
EditorLayer::EditorLayer()
	: Layer("EditorLayer")
{
	
}

void EditorLayer::OnAttach()
{
	
	InitScene();
	m_Scene = make_shared<Scene>();
	m_EditorCamera.SetPosition(glm::vec3(0.0f, 9.0f, 12.0f));
	Renderer3D::Init();
	m_ScenePanel.AttachToScene(m_Scene);
	m_PropertiesPanel.SetReference(&m_ScenePanel);
	
}

void EditorLayer::InitScene() {
	
	
}

void EditorLayer::OnDetach()
{
	Renderer3D::ShutDown();
}

void EditorLayer::OnUpdate(XRE::TimeStep ts)
{
		
	//Temp：更新场景属性
	UpdateScene();
	m_EditorCamera.OnUpdate(ts);
	//场景处理
	//m_Scene->OnUpdate(ts);

	m_Scene->OnUpdateEditing(ts,m_EditorCamera);

	
	
	

}

void EditorLayer::UpdateScene()
{

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
		if (ImGui::BeginMenu(u8"文件"))
		{

			if (ImGui::MenuItem(u8"打开")) {
				m_Scene = make_shared<Scene>();
				auto path = FileDialogs::OpenFile("Scene (*.scene)\0*.scene\0");
				m_Scene->Deserialize(path);
				m_ScenePanel.AttachToScene(m_Scene);
			}
			if (ImGui::MenuItem(u8"保存")) m_Scene->Save();
			if (ImGui::MenuItem(u8"另存为")) {
				
				auto path = FileDialogs::SaveFile("Scene (*.scene)\0*.scene\0");
				m_Scene->Serialize(path);
			}
				
				
			if (ImGui::MenuItem(u8"退出")) XRE::Application::GetApplication().Close();
			
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Begin(u8"设置");
		ImGui::Text("FPS:%d", Application::GetFPS());
		ImGui::Separator();

		if (ImGui::Button("ReloadShader")) {
			Renderer3D::Init();
		}
		
		uint32_t mapID = Renderer3D::m_ShadowFrameBuffer->GetDepthAttachment();

		ImGui::Separator();
		ImGui::Text("DepthMap");
		ImGui::Image((void*)mapID, ImVec2{ 300, 300 }, ImVec2{ 0,1 }, ImVec2{ 1,0 }) ;

	ImGui::End();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	
	



	ImGui::Begin(u8"视窗");
		


		ImGui::Checkbox(u8"查看Gizmos", &m_ShowGizmos);ImGui::SameLine();

		
		ImGui::RadioButton(u8"移动", &m_GizmoType, ImGuizmo::OPERATION::TRANSLATE); ImGui::SameLine();
		ImGui::RadioButton(u8"旋转", &m_GizmoType, ImGuizmo::OPERATION::ROTATE); ImGui::SameLine();
		ImGui::RadioButton(u8"缩放", &m_GizmoType, ImGuizmo::OPERATION::SCALE); 

		
		auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

		m_ViewportFocused = ImGui::IsWindowFocused();
		
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::GetApplication().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);


		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			//XRE_CORE_TRACE("viewport: {0} x {1} ", viewportPanelSize.x, viewportPanelSize.y);
			Renderer3D::m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_EditorCamera.SetViewPortSize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		uint32_t textureID = Renderer3D::m_FrameBuffer->GetColorAttachment(0);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + m_ViewportSize.x, minBound.y + m_ViewportSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		GameObject selectedEntity = m_ScenePanel.GetSelected();
		if (selectedEntity && m_GizmoType != -1 && m_ShowGizmos)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
			
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(XRE_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = glm::degrees(rotation) - tc.m_Rotation;
				tc.m_Translation = translation;
				tc.m_Rotation += deltaRotation;
				tc.m_Scale = scale;
			}
		}
	ImGui::End();
	ImGui::PopStyleVar();

	m_ScenePanel.OnImGuiRender();
	m_PropertiesPanel.OnImGuiRender();

	ImGui::Begin(u8"资产管理器");
	ImGui::End();

	//ImGui::ShowDemoWindow();

	ImGui::End();
		

}



void EditorLayer::OnEvent(XRE::Event& e)
{
	m_EditorCamera.OnEvent(e);
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyReleased));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	
}

bool EditorLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	
	switch (e.GetKeyCode()) {
		// Gizmos
	case XRE_KEY_U:
		m_GizmoType = -1;
		break;
	case XRE_KEY_I:
		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		break;
	case XRE_KEY_O:
		m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		break;
	case XRE_KEY_P:
		m_GizmoType = ImGuizmo::OPERATION::SCALE;
		break;
	}
	return false;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if(!ImGuizmo::IsOver())
	if (e.GetMouseButton() == XRE_MOUSE_BUTTON_LEFT) {
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData;
			Renderer3D::m_FrameBuffer->ReadPixel(1, mouseX, mouseY,&pixelData);
			m_ScenePanel.Select(pixelData);

		}
	}
	

	return false;
}



