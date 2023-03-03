
#include "pch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
namespace XRE {

	//Declaration of Static Variables
	Ref<Shader> Renderer3D::defaultObjShader;
	Ref<Shader> Renderer3D::defaultPBRShader;
	Ref<Shader> Renderer3D::simpleDepthShader;
	Ref<Shader> Renderer3D::postShader;
	Ref<Shader> Renderer3D::activeShader;
	Ref<Texture2D> Renderer3D::defaultAlbedo;

	Ref<SkyBox> Renderer3D::m_SkyBox;
	Ref<Framebuffer> Renderer3D::m_FrameBuffer;
	Ref<Framebuffer> Renderer3D::m_ShadowFrameBuffer;
	Ref<Framebuffer> Renderer3D::m_PostFrameBuffer;
	Ref<VertexArray> Renderer3D::m_Quad;

	Light Renderer3D::m_Light;

	glm::mat4  Renderer3D::m_VP, Renderer3D::m_LVP;
	void Renderer3D::Init()
	{
		defaultObjShader = ResourceManager::GetShader("../Assets/shaders/default.glsl");

		defaultPBRShader = ResourceManager::GetShader("../Assets/shaders/pbr_object.glsl");
		//	Shader::Create("assets/shaders/default.glsl");
		simpleDepthShader = ResourceManager::GetShader("../Assets/shaders/depth.glsl");

		postShader = ResourceManager::GetShader("../Assets/shaders/postprocessing.glsl");

		defaultAlbedo = ResourceManager::GetTex2D("../Assets/textures/albedo.jpg");

		activeShader = defaultObjShader;
			//Texture2D::Create("assets/textures/albedo.jpg");
		m_FrameBuffer = Framebuffer::Create(1280, 720);
		m_PostFrameBuffer = Framebuffer::Create(1280, 720);
		m_ShadowFrameBuffer = Framebuffer::Create(2048, 2048);

		m_SkyBox = make_shared<SkyBox>();


		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};

		uint32_t Indices[6] = {
			0,1,2,
			0,2,3
		};
		

		BufferLayout layout = {
				{ ShaderDataType::Float2, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" }
		};

		m_Quad = VertexArray::Create();

		//2.1 setup VBO
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
		vertexBuffer->SetLayout(layout);
		m_Quad->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(Indices,6));
		m_Quad->SetIndexBuffer(indexBuffer);

		
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


	void Renderer3D::CullFace(bool b)
	{
		RenderCommand::CullFace(b);
	}
	
	void Renderer3D::EndScene()
	{

	}

	void Renderer3D::Clear()
	{
		RenderCommand::Clear();
	}

	void Renderer3D::SetClearColor(glm::vec4 color)
	{
		RenderCommand::SetClearColor(color);
	}

	void Renderer3D::StartShadowPass()
	{
		m_ShadowFrameBuffer->Bind();

		RenderCommand::Clear();
		//Ref<OrthographicCamera> dirLight_Camera = make_shared<OrthographicCamera>(-10.0f, 10.0f, -10.0f, 10.0f, m_Light.getDirLight()->m_Direction);
		Ref<Camera> dirLight_Camera = make_shared<Camera>(Orthographic, m_Light.getDirLight().m_Rotation);
		m_LVP = dirLight_Camera->GetViewProjectionMatrix();
		// render scene from light's point of view
		activeShader = simpleDepthShader;
		activeShader->Bind();
		activeShader->SetMat4("lightSpaceMatrix", m_LVP);
		StartScene(dirLight_Camera);
		//Renderer3D::StartScene(camera);
		CullFace(true);
	}

	void Renderer3D::EndShadowPass()
	{
		CullFace(false);
		EndScene();
		activeShader = defaultObjShader;
		m_ShadowFrameBuffer->Unbind();
	}

	void Renderer3D::SetShadowMapOfActive(uint32_t slot)
	{
		m_ShadowFrameBuffer->ActiveDepth(slot);
		activeShader->SetInt("shadowMap", slot);
		activeShader->SetMat4("lightSpaceMatrix", m_LVP);
	}

	void Renderer3D::PostProcessing()
	{
		RenderCommand::Clear();
		m_PostFrameBuffer ->Resize(m_FrameBuffer->GetWidth(),m_FrameBuffer->GetHeight());
		m_PostFrameBuffer->Bind();
		postShader->Bind();
		m_FrameBuffer->ActiveColor(0);
		postShader->SetInt("screenTexture", 0);
		RenderCommand::DrawIndexed(m_Quad);

		

	}
	
	void Renderer3D::DrawLight()
	{
		m_Light.Draw(activeShader);
	}
	void Renderer3D::DrawModel(const Ref<Model> model, glm::mat4 transform)
	{
		if(model)
		for (auto mesh : model->m_Meshes) {
			mesh.BindMaterial(activeShader);
			activeShader->Bind();
			activeShader->SetMat4("u_Transform", transform);
			mesh.GetVAO()->Bind();
			RenderCommand::DrawIndexed(mesh.GetVAO());


		}
		//model->Draw(defaultObjShader, transform);
	}
	void Renderer3D::DrawSkybox()
	{
		m_SkyBox->GetShader()->Bind();
		
		//skybox->GetShader()->SetInt("skybox", skybox->GetCubemapTexture());
		m_SkyBox->GetShader()->SetMat4("u_ViewProjection", m_VP);
		m_SkyBox->GetVAO()->Bind();
		RenderCommand::DrawSkyIndexed(m_SkyBox->GetVAO(), m_SkyBox->GetCubemapTexture());

	}
}