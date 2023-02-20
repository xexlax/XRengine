#include "Sandbox3D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#pragma once

Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox3D::OnAttach()
{
	m_NanosuitModel.reset(new Model("./assets/models/nanosuit.obj"));
	auto defaultShader = m_ShaderLibrary.Load("assets/shaders/default.glsl");

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

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(XRE::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);
	Ref<Camera> camera = m_CameraController.GetCamera();
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	// Render
	XRE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	XRE::RenderCommand::Clear();

	XRE::Renderer::BeginScene(m_CameraController.GetCamera());




	std::dynamic_pointer_cast<XRE::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<XRE::OpenGLShader>(m_FlatColorShader)->setFloat4("u_Color", m_SquareColor);

	XRE::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));











	auto defaultShader = m_ShaderLibrary.Get("default");
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->Bind();
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("objectColor", glm::vec3(0.8f, 0.5f, 0.2f));
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("lightPos", lightPos);
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("viewPos", camera->GetPosition());


	// view/projection transformations
	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::mat4 view = camera->GetViewMatrix();
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setMat4("projection", projection);
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setMat4("view", view);

	// world transformation
	glm::mat4 model = glm::mat4(1.0f);
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setMat4("model", model);

	m_NanosuitModel->Draw(defaultShader);

	
	XRE::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox3D::OnEvent(XRE::Event& e)
{
	m_CameraController.OnEvent(e);
}
