#pragma once

#include "xre.h"

class Sandbox2D : public XRE::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(XRE::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(XRE::Event& e) override;
private:
	XRE::OrthographicCameraController m_CameraController;

	// Temp
	XRE::Ref<XRE::VertexArray> m_SquareVA;
	XRE::Ref<XRE::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};