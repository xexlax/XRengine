#pragma once

#include "xre.h"

using namespace XRE;
class Sandbox3D : public XRE::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(XRE::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(XRE::Event& e) override;
private:
	//OrthographicCameraController m_CameraController;
	PerspectiveCameraController m_CameraController;
	// Temp
	Ref<Model> m_NanosuitModel;

	XRE::Ref<XRE::VertexArray> m_SquareVA;
	XRE::Ref<XRE::Shader> m_FlatColorShader;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};