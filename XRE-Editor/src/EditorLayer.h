#pragma once

#include "xre.h"

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
	void InitScene();
	void SetScene();
private:
	
	PerspectiveCameraController m_CameraController;

	Scene m_Scene;
	
	GameObject SceneCameraGO;
	GameObject PointLight1GO, PointLight2GO, DirLightGO;
	GameObject CubeGO, CylinderGO, CapsuleGO, SphereGO, FloorGO, NanoGO, BocchiGO;

	//IMGUI TESTING BOARD ARGS
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
};