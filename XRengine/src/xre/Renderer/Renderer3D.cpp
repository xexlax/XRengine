
#include "pch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
namespace XRE {
	Ref<Shader> Renderer3D::defaultObjShader;
	Ref<Shader> Renderer3D::defaultPBRShader;
	Ref<Shader> Renderer3D::activeShader;
	Ref<Texture2D> Renderer3D::defaultAlbedo;
	glm::mat4  Renderer3D::m_VP;
	void Renderer3D::Init()
	{
		defaultObjShader = ResourceManager::GetShaderLib()->Load("../Assets/shaders/default.glsl");

		defaultPBRShader = ResourceManager::GetShaderLib()->Load("../Assets/shaders/pbr_object.glsl");
		//	Shader::Create("assets/shaders/default.glsl");


		defaultAlbedo = ResourceManager::GetTex2DLib()->Load("../Assets/textures/albedo.jpg");

		activeShader = defaultObjShader;
			//Texture2D::Create("assets/textures/albedo.jpg");

	}
	void Renderer3D::ShutDown()
	{
	}
	void Renderer3D::StartScene(const Ref<Camera> camera)
	{
		activeShader->Bind();
		m_VP = camera->GetProjectionMatrix() * glm::mat4(glm::mat3(camera->GetViewMatrix())) ;
		activeShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		activeShader->SetFloat3("viewPos", camera->GetPosition());
	}
	
	void Renderer3D::EndScene()
	{
	}
	void Renderer3D::PBROn(bool on)
	{
		if (on) {

			activeShader = defaultPBRShader;

		}
		else
		{
			activeShader = defaultObjShader;
		}
	}
	void Renderer3D::DrawLight(const Ref<Light> light)
	{
		light->Draw(activeShader);
	}
	void Renderer3D::DrawModel(const Ref<Model> model, glm::mat4 transform)
	{

		for (auto mesh : model->m_Meshes) {
			mesh.BindMaterial(activeShader);
			activeShader->Bind();
			activeShader->SetMat4("u_Transform", transform);
			mesh.GetVAO()->Bind();
			RenderCommand::DrawIndexed(mesh.GetVAO());


		}
		//model->Draw(defaultObjShader, transform);
	}
	void Renderer3D::DrawSkybox(const Ref<SkyBox> skybox)
	{
		skybox->GetShader()->Bind();
		
		//skybox->GetShader()->SetInt("skybox", skybox->GetCubemapTexture());
		skybox->GetShader()->SetMat4("u_ViewProjection", m_VP);
		skybox->GetVAO()->Bind();
		RenderCommand::DrawSkyIndexed(skybox->GetVAO(),skybox->GetCubemapTexture());

	}
}