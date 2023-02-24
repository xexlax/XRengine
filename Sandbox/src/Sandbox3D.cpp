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
	

	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/cube.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/cylinder.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/sphere.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/capsule.obj"));
	m_Models.emplace_back(std::make_shared<Model>("../Assets/models/nanosuit/nanosuit.obj"));

	Ref<PointLight> p1 = std::make_shared<PointLight>(glm::vec3(1.2f, 2.5f, 0.0f));
	Ref<PointLight> p2 = std::make_shared<PointLight>(glm::vec3(-1.2f, 3.5f, 3.0f));
	Ref<DirectionalLight> dl = std::make_shared<DirectionalLight>(glm::vec3(-0.2f, -1.0f, 0.3f));
	m_Light.AddPLight(p1);
	//m_Light.AddPLight(p2);
	m_Light.SetDirLight(dl);

	Renderer3D::Init();
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(XRE::TimeStep ts)
{
	
		m_CameraController.OnUpdate(ts);

	Ref<Camera> camera = m_CameraController.GetCamera();
	m_Framebuffer->Bind();
	// Render
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer3D::StartScene(m_CameraController.GetCamera());


	m_Light.getPointLight(0)->m_Color = m_SquareColor;
	m_Light.getDirLight()->m_Color = m_color2;



	Renderer3D::DrawLight(make_shared<Light>(m_Light));

	glm::mat4 transform = glm::mat4(1.0f);
	for (int i = 0;i < 4;i++) {
		transform = glm::translate(transform, glm::vec3(2.0f, 0.0f, 0.0f));
		Renderer3D::DrawModel(m_Models[i], transform);

	}
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
	Renderer3D::DrawModel(m_Models[4], transform);
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 0.2f, 20.0f));
	transform = glm::translate(transform, glm::vec3(0.0f, -3.0f, 0.0f));
	Renderer3D::DrawModel(m_Models[0], transform);
	XRE::Renderer3D::EndScene();
	m_Framebuffer->Unbind();
	
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color1", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Color2", glm::value_ptr(m_color2));
	ImGui::End();

	
		
	
	
}

void Sandbox3D::OnEvent(XRE::Event& e)
{
	m_CameraController.OnEvent(e);
	
}
