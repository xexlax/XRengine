#pragma once

#include "XRE.h"
#include "XRE_Editor.h"
#include "imgui/imgui.h"
#include "ImGuizmo.h"
using namespace XRE;
class EditorLayer : public XRE::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(TimeStep ts) override;
	void OnEvent(Event& e) override;
	void OnImGuiRender() override;





private:

	void OnSceneReload();
	bool OnKeyReleased(KeyReleasedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	void ToolBar();
	void DetectCommand();
private:
	//Editor Panels

	ScenePanel m_ScenePanel;
	ActionPanel m_ActionPanel;
	AssetsPanel m_AssetsPanel;
	PropertiesPanel m_PropertiesPanel;
	XRef<Texture2D> m_IconPlay, m_IconStop, m_IconPause, m_IconSimulate;
	

	//Test Scene
	SceneStatus m_Status = Editing;
	XRef<Scene> m_Scene;
	EditorCamera m_EditorCamera;
	

	//IMGUI TESTING BOARD ARGS
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];


	//Gizmos Status
	int m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
	bool m_ShowGizmos= true;
	bool m_GizmosUsing;

};