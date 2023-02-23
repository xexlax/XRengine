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

	void OnUpdate(XRE::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(XRE::Event& e) override;
private:
	//OrthographicCameraController m_CameraController;
	PerspectiveCameraController m_CameraController;

	// Temp
	vector<Ref<Model>> m_Models;

	Light m_Light;
	Ref<Framebuffer> m_Framebuffer;
	Ref<VertexArray> m_SquareVA;
	Ref<Shader> m_FlatColorShader;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f }, m_color2 = { 0.8f,0.2f,0.3f,1.0f };
};