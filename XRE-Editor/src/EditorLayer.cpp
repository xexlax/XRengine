#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorPanels\PanelsManager.h"

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
	
	
	m_Scene = make_shared<Scene>();
	m_EditorCamera.SetPosition(glm::vec3(0.0f, 9.0f, 12.0f));
	Renderer3D::Init();
	PanelsManager::Init();
	PanelsManager::GetScenePanel()->AttachToScene(m_Scene);
	PanelsManager::GetPropertiesPanel()->SetEC(&m_EditorCamera);
	m_IconPlay = ResourceManager::GetTex2D(AssetsDirectory"textures/Play.png");
	m_IconStop = ResourceManager::GetTex2D(AssetsDirectory"textures/Stop.png");
	m_IconPause = ResourceManager::GetTex2D(AssetsDirectory"textures/Pause.png");
	m_IconSimulate = ResourceManager::GetTex2D(AssetsDirectory"textures/Simulate.png");
	
	
	
}



void EditorLayer::OnDetach()
{
	Renderer3D::ShutDown();
}

void EditorLayer::OnUpdate(XRE::TimeStep ts)
{
		

	//场景处理
	//m_Scene->OnUpdate(ts);

	switch (m_Status)
	{
	case SceneStatus::Editing:

		m_EditorCamera.OnUpdate(ts);

		m_Scene->OnUpdateEditing(ts, m_EditorCamera);

		break;
	case SceneStatus::Runtime:
		m_Scene->OnUpdateRuntime(ts);

		break;

	case SceneStatus::Paused:

		break;
	default:
		break;
	};
	

	
	
	

}

void EditorLayer::ToolBar()
{
	


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##Tools");

	ImGui::Checkbox(u8"查看Gizmos", &m_ShowGizmos);ImGui::SameLine();


	ImGui::RadioButton(u8"移动", &m_GizmoType, ImGuizmo::OPERATION::TRANSLATE); ImGui::SameLine();
	ImGui::RadioButton(u8"旋转", &m_GizmoType, ImGuizmo::OPERATION::ROTATE); ImGui::SameLine();
	ImGui::RadioButton(u8"缩放", &m_GizmoType, ImGuizmo::OPERATION::SCALE); ImGui::SameLine();

	float size = ImGui::GetTextLineHeightWithSpacing();
	XRef<Texture2D> icon1 = ((m_Status == SceneStatus::Editing) ? m_IconPlay : m_IconStop);
	XRef<Texture2D> icon2 = ((m_Status == SceneStatus::Paused) ? m_IconPlay : m_IconPause);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - size);
	//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
	if (ImGui::ImageButton((ImTextureID)icon1->GetRendererId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
	{
		if (m_Status == SceneStatus::Editing) {
			m_Status = SceneStatus::Runtime;
			PanelsManager::GetScenePanel()->UnSelect();
			m_Scene->OnRuntimeBegin();
		}

		else if (m_Status == SceneStatus::Runtime || m_Status == SceneStatus::Paused) {
			m_Status = SceneStatus::Editing;
			m_Scene->OnRuntimeEnd();
		}

	

	}
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f );

	if (m_Status == SceneStatus::Editing) ImGui::BeginDisabled();
	if (ImGui::ImageButton((ImTextureID)icon2->GetRendererId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
	{
		if (m_Status == SceneStatus::Runtime) {
			m_Status = SceneStatus::Paused;
		}
		else 
		if (m_Status == SceneStatus::Paused) {
			m_Status = SceneStatus::Runtime;
		}

	}
	if (m_Status == SceneStatus::Editing) ImGui::EndDisabled();

	ImGui::End();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);;

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
			if (ImGui::MenuItem(u8"新场景")) {

				m_Scene = XMakeRef<Scene>();
				OnSceneReload();
			}

			if (ImGui::MenuItem(u8"打开")) {

				if (m_Status == SceneStatus::Runtime || m_Status == SceneStatus::Paused) {
					m_Status = SceneStatus::Editing;
					m_Scene->OnRuntimeEnd();
				}
				m_Scene = make_shared<Scene>();
				auto path = FileDialogs::OpenFile("Scene (*.scene)\0*.scene\0");
				if (path != "") {
					m_Scene->Deserialize(path);
					OnSceneReload();
				}
				
				
			
			}

			if (ImGui::MenuItem(u8"保存")) {
				if (m_Scene->GetFilePath() == "") {
					auto path = FileDialogs::SaveFile("Scene (*.scene)\0*.scene\0");
					m_Scene->Serialize(path);
				}
				else
					m_Scene->Save();
			}
				
				
				
			if (ImGui::MenuItem(u8"另存为")) {
				
				auto path = FileDialogs::SaveFile("Scene (*.scene)\0*.scene\0");
				m_Scene->Serialize(path);
			}
				
				
			if (ImGui::MenuItem(u8"退出")) XRE::Application::GetApplication().Close();
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"编辑"))
		{
			if (ImGui::MenuItem(u8"撤销")) {
				CommandManager::Get().Undo();
			}

			if (ImGui::MenuItem(u8"重做")) {
				CommandManager::Get().Redo();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ToolBar();

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
		

		//ToolBar();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		
		
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
		ImGui::Image((ImTextureID)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
			{
				std::string path = *(std::string*)payload->Data;

				if (path.find(".scene") != string::npos) {

					if (m_Status == SceneStatus::Runtime || m_Status == SceneStatus::Paused) {
						m_Status = SceneStatus::Editing;
						m_Scene->OnRuntimeEnd();
					}
					m_Scene = make_shared<Scene>();


					m_Scene->Deserialize(path);
					OnSceneReload();
				}

				if (path.find(".obj") != string::npos) {

					std::string name;
					size_t pos1 = path.find_last_of('\\');
					size_t pos2 = path.find_last_of('.');
					if (pos1 != std::string::npos) {
						if(pos2!= string::npos)
							name = path.substr( pos1+1,pos2 - pos1-1);
						else
						name = path.substr(pos1+1);
					}

					auto go = m_Scene->CreateGameObject(name);
					go.AddComponent<MeshRendererComponent>(ResourceManager::GetModel(path));
					CommandManager::Get().Command_CreateObj(go);

				}

				if (path.find(".go") != string::npos) {	
					m_Scene->CreateFromFile(path);
				}

				
				if (path.find(".mat") != string::npos) {

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
						
						Renderer3D::m_FrameBuffer->ReadPixel(1, mouseX, mouseY, &pixelData);
						PanelsManager::GetScenePanel()->Select(pixelData);
						if (PanelsManager::GetScenePanel()->GetSelected().HasComponent<MeshRendererComponent>()) {
							PanelsManager::GetScenePanel()->GetSelected().GetComponent<MeshRendererComponent>().m_Materials[0]= ResourceManager::GetMaterial(path);
						}
						

					}
						

				}

				
				
				
			}
			ImGui::EndDragDropTarget();
		}

		//Draw Gizmo
		if (m_Status != SceneStatus::Runtime) {

			GameObject selectedEntity = PanelsManager::GetScenePanel()->GetSelected();
			if (selectedEntity && m_GizmoType != -1 && m_ShowGizmos)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

			
				ImGuizmo::SetRect(m_ViewportBounds[0].x,m_ViewportBounds[0].y, 
					m_ViewportBounds[1].x- m_ViewportBounds[0].x, m_ViewportBounds[1].y- m_ViewportBounds[0].y);
				 
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetGlobalTransform();

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
					//auto p = tc.parent;
					//selectedEntity.SetParent(nullptr);
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = glm::degrees(rotation) - tc.GetGlobalEuler();
					tc.SetGlobalTranslation(translation);
					tc.SetGlobalRotation(tc.GetGlobalEuler() + deltaRotation);
					tc.SetGlobalScale(scale);

					//selectedEntity.SetParent(*p);
					
				}
			}
		
		}
	ImGui::End();
	ImGui::PopStyleVar();

	PanelsManager::OnImGuiRender();



	//ImGui::ShowDemoWindow();

	ImGui::End();

	if(PanelsManager::GetScenePanel()->GetSelected()&&m_GizmoType!=-1)
	DetectCommand();

	CommandManager::Get().OnUpdate();
		

}

void EditorLayer::DetectCommand() {
	
	if (!m_GizmosUsing && ImGuizmo::IsUsing()) {
		m_GizmosUsing = true;
		glm::vec3* ptr;
		if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE) ptr = &PanelsManager::GetScenePanel()->GetSelected().GetComponent<TransformComponent>().m_Translation;
		if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) ptr = &PanelsManager::GetScenePanel()->GetSelected().GetComponent<TransformComponent>().m_Rotation;
		if (m_GizmoType == ImGuizmo::OPERATION::SCALE) ptr = &PanelsManager::GetScenePanel()->GetSelected().GetComponent<TransformComponent>().m_Scale;

		CommandManager::Get().Begin_Command_Edit<glm::vec3>(ptr, *ptr);

	}

	if (m_GizmosUsing && !ImGuizmo::IsUsing()) {
		m_GizmosUsing = false;

		glm::vec3* ptr;


		if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE) ptr = &PanelsManager::GetScenePanel()->GetSelected().GetComponent<TransformComponent>().m_Translation;
		if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) ptr = &PanelsManager::GetScenePanel()->GetSelected().GetComponent<TransformComponent>().m_Rotation;
		if (m_GizmoType == ImGuizmo::OPERATION::SCALE) ptr = &PanelsManager::GetScenePanel()->GetSelected().GetComponent<TransformComponent>().m_Scale;

		CommandManager::Get().End_Command_Edit<glm::vec3>(ptr, *ptr);
	}
}

void EditorLayer::OnEvent(XRE::Event& e)
{
	m_EditorCamera.OnEvent(e);
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyReleased));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	
}

void EditorLayer::OnSceneReload()
{
	PanelsManager::GetScenePanel()->AttachToScene(m_Scene);
	m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	CommandManager::Get().Clear();
}

bool EditorLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	
	switch (e.GetKeyCode()) {
		// Gizmos
	case XRE_KEY_Z:
		if (Input::IsKeyPressed(XRE_KEY_LEFT_CONTROL))
			CommandManager::Get().Undo();
		break;
	case XRE_KEY_Y:
		if (Input::IsKeyPressed(XRE_KEY_LEFT_CONTROL))
			CommandManager::Get().Redo();
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
			PanelsManager::GetScenePanel()->Select(pixelData);

		}
	}
	

	return false;
}



