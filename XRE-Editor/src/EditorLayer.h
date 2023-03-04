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
	bool OnKeyReleased(KeyReleasedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	void InitScene();
	void SetScene();
private:
	//Editor Panels

	ScenePanel m_ScenePanel;
	PropertiesPanel m_PropertiesPanel;

	
	//Test Scene
	SceneStatus m_Status = Editing;
	Ref<Scene> m_Scene;
	EditorCamera m_EditorCamera;
	glm::vec2 m_ViewportBounds[2];
	GameObject SceneCameraGO;
	GameObject PointLight1GO, PointLight2GO, DirLightGO;
	GameObject CubeGO, CylinderGO, CapsuleGO, SphereGO, FloorGO, NanoGO, BocchiGO;

	//IMGUI TESTING BOARD ARGS
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	int m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
	bool m_ShowGizmos= true;


};