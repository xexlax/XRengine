#include "Sandbox3D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include "GLFW\include\GLFW\glfw3.h"
#pragma once
#define PI 3.1415926f
Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f , glm::vec3(0.0f,2.0f,-5.0f))
{
	
}

void Sandbox3D::OnAttach()
{

	//m_NanosuitModel.reset(new Model("./assets/models/nanosuit/nanosuit.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/cube.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/cylinder.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/sphere.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/capsule.obj"));
	m_Models.emplace_back(std::make_shared<Model>("./assets/models/nanosuit/nanosuit.obj"));
	
	auto defaultShader = ResourceManager::GetShaderLib()->Load("assets/shaders/default.glsl");

	Ref<PointLight> p1= std::make_shared<PointLight>(glm::vec3(1.2f, 2.5f, 0.0f));
	Ref<DirectionalLight> dl= std::make_shared<DirectionalLight>(glm::vec3(-0.2f, -1.0f, 0.3f));
	m_Light.AddPLight(p1);
	m_Light.SetDirLight(dl);
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(XRE::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);
	Ref<Camera> camera = m_CameraController.GetCamera();
	
	// Render
	XRE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	XRE::RenderCommand::Clear();

	XRE::Renderer::BeginScene(m_CameraController.GetCamera());


	m_Light.getPointLight(0)->m_Color = m_SquareColor;
	m_Light.getDirLight()->m_Color = m_SquareColor;

	auto defaultShader = ResourceManager::GetShaderLib()->Get("default");
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->Bind();
	std::dynamic_pointer_cast<XRE::OpenGLShader>(defaultShader)->setFloat3("viewPos", camera->GetPosition());

	m_Light.Draw(defaultShader);
	
	glm::mat4 transform = glm::mat4(1.0f);
	for (int i = 0;i < 4;i++) {
		transform = glm::translate(transform, glm::vec3(2.0f, 0.0f, 0.0f));
		m_Models[i]->Draw(defaultShader, transform);
		
	}
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
	//transform = glm::rotate(transform, PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	m_Models[4]->Draw(defaultShader, transform);
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f,0.2f,20.0f));
	transform = glm::translate(transform, glm::vec3(0.0f,-3.0f, 0.0f));
	m_Models[0]->Draw(defaultShader, transform);
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
