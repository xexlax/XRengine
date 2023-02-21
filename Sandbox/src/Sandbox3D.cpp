#include "Sandbox3D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#pragma once
#define PI 3.1415926f
Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f , glm::vec3(0.0f,2.0f,-5.0f))
{
}

void Sandbox3D::OnAttach()
{
	m_NanosuitModel.reset(new Model("./assets/models/nanosuit/nanosuit.obj"));
	auto defaultShader = ResourceManager::GetShaderLib()->Load("assets/shaders/default.glsl");

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





	auto defaultShader = ResourceManager::GetShaderLib()->Get("default");
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->Bind();
	//std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("objectColor", m_SquareColor);
	
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	
	
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("light.direction",glm::vec3(-0.2f, -1.0f, 0.3f));
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("viewPos", camera->GetPosition());

	glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
	transform = glm::rotate(transform, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	m_NanosuitModel->Draw(defaultShader,transform);

	
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
