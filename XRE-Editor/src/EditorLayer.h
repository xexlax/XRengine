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
private:
	//OrthographicCameraController m_CameraController;
	PerspectiveCameraController m_CameraController;
	
	// Temp
	vector<Ref<Model>> m_Models;

	Light m_Light;
	Ref<SkyBox> m_Skybox;

	Ref<Framebuffer> m_Framebuffer;
	Ref<VertexArray> m_SquareVA;
	Ref<Shader> m_FlatColorShader;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	bool m_ShowSkybox = true;
	bool m_PBR = false;
	float m_Metallic= 0.2f, m_Roughness = 0.2f;
	float m_DirLightIntensity = 2.0f;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.9f, 1.0f }, 
		m_color2 = { 0.6f,0.4f,0.3f,1.0f },
		m_color3 = { 0.8f,0.4f,0.2f,1.0f };
};