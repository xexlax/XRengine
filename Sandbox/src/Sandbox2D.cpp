#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platforms/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = XRE::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	XRE::Ref<XRE::VertexBuffer> squareVB;
	squareVB.reset(XRE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ XRE::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	XRE::Ref<XRE::IndexBuffer> squareIB;
	squareIB.reset(XRE::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = XRE::Shader::Create("assets/shaders/flatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(XRE::TimeStep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	XRE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	XRE::RenderCommand::Clear();

	XRE::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<XRE::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<XRE::OpenGLShader>(m_FlatColorShader)->setFloat4("u_Color", m_SquareColor);

	XRE::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	XRE::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(XRE::Event& e)
{
	m_CameraController.OnEvent(e);
}