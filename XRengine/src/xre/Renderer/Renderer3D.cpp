
#include "pch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"

namespace XRE {

	//Declaration of Static Variables
	XRef<Shader> Renderer3D::defaultObjShader;
	XRef<Shader> Renderer3D::defaultPBRShader;
	XRef<Shader> Renderer3D::simpleDepthShader;
	XRef<Shader> Renderer3D::flatColorShader;
	XRef<Shader> Renderer3D::postShader;
	XRef<Shader> Renderer3D::activeShader;
	XRef<Texture2D> Renderer3D::defaultAlbedo;

	XRef<SkyBox> Renderer3D::m_SkyBox;
	XRef<Framebuffer> Renderer3D::m_FrameBuffer;
	XRef<Framebuffer> Renderer3D::m_ShadowFrameBuffer;
	XRef<Framebuffer> Renderer3D::m_PostFrameBuffer;
	XRef<VertexArray> Renderer3D::m_Quad;

	Light Renderer3D::m_Light;

	glm::mat4  Renderer3D::m_VP, Renderer3D::m_LVP;
	void Renderer3D::Init()
	{
		
		defaultObjShader = ResourceManager::GetShader("../Assets/shaders/default.glsl");

		defaultPBRShader = ResourceManager::GetShader("../Assets/shaders/pbr_object.glsl");
		//	Shader::Create("assets/shaders/default.glsl");
		simpleDepthShader = ResourceManager::GetShader("../Assets/shaders/depth.glsl");

		flatColorShader = ResourceManager::GetShader("../Assets/shaders/flatColor.glsl");

		postShader = ResourceManager::GetShader("../Assets/shaders/postprocessing.glsl");

		defaultAlbedo = ResourceManager::GetTex2D("../Assets/textures/albedo.jpg");

		activeShader = defaultObjShader;
			//Texture2D::Create("assets/textures/albedo.jpg");

		FramebufferSpecification fb ;
		fb.Width = 768;
		fb.Height = 1024;
		fb.Attachments = 
		{ 
			FramebufferTextureFormat::RGBA8,
			FramebufferTextureFormat::RED_INTEGER, 
			FramebufferTextureFormat::Depth 
		};

		FramebufferSpecification sfb;

		sfb.Width = 2048;
		sfb.Height = 2048;
		sfb.Attachments = { FramebufferTextureFormat::Depth };


		m_FrameBuffer = Framebuffer::Create(fb);
		m_PostFrameBuffer = Framebuffer::Create(fb);
		m_ShadowFrameBuffer = Framebuffer::Create(sfb);

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
		XRef<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
		vertexBuffer->SetLayout(layout);
		m_Quad->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		XRef<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(Indices,6));
		m_Quad->SetIndexBuffer(indexBuffer);

		
	}
	void Renderer3D::ShutDown()
	{
	}
	void Renderer3D::StartScene(const XRef<Camera> camera)
	{
		activeShader->Bind();
		m_VP = camera->GetProjectionMatrix() * glm::mat4(glm::mat3(camera->GetViewMatrix())) ;
		activeShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		activeShader->SetFloat3("viewPos", camera->GetPosition());

		flatColorShader->Bind();
		flatColorShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
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
		//XRef<OrthographicCamera> dirLight_Camera = make_shared<OrthographicCamera>(-10.0f, 10.0f, -10.0f, 10.0f, m_Light.getDirLight()->m_Direction);
		XRef<Camera> dirLight_Camera = make_shared<Camera>(Orthographic, m_Light.getDirLight().m_Rotation);
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
	void Renderer3D::DrawModel(const XRef<Model> model,const glm::mat4& transform)
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
	void Renderer3D::DrawShapeFrame(const PhysicsShape& Shape, const glm::mat4& transform)
	{
		RenderCommand::SetDepthTest(false);

		flatColorShader->Bind();
		flatColorShader->SetFloat4("u_Color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));


		
		
		if (Shape.m_Type == PhysicsShape::Box) {
			
			glm::mat4 scale = glm::scale(Shape.GetLocalTransform(), Shape.m_Size);
			flatColorShader->SetMat4("u_Transform",  transform*scale);
			RenderCommand::DrawIndexedLines(VertexArray::GetCubeVA());

		}
		else if (Shape.m_Type == PhysicsShape::Sphere) {
			
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(Shape.m_Size.x));
			flatColorShader->SetMat4("u_Transform", Shape.GetLocalTransform()*  transform* scale);
			RenderCommand::DrawIndexedLineLoop(VertexArray::GetCircleVA());

			glm::mat4 rot =glm::toMat4(glm::quat(glm::radians(glm::vec3(90, 0, 0))));
			flatColorShader->SetMat4("u_Transform",  Shape.GetLocalTransform() * transform * rot * scale);
			RenderCommand::DrawIndexedLineLoop(VertexArray::GetCircleVA());

			rot =  glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 0, 90))));
			flatColorShader->SetMat4("u_Transform", Shape.GetLocalTransform() * transform * rot * scale);
			RenderCommand::DrawIndexedLineLoop(VertexArray::GetCircleVA());

		}
		else if (Shape.m_Type == PhysicsShape::Capsule) {
			
			glm::mat4 subrot = glm::toMat4(glm::quat(glm::radians(Shape.m_Rotation)));
			glm::mat4 subtrans = glm::translate(glm::mat4(1.0f), Shape.m_Offset);
			flatColorShader->SetMat4("u_Transform", transform*subtrans * subrot);
			RenderCommand::DrawIndexedLineLoop(VertexArray::GetCapsuleVA(Shape.m_Size.x, Shape.m_Size.y));

			 
			glm::mat4 rot = glm::toMat4(glm::quat(glm::radians(glm::vec3(0, 90, 0))));
			flatColorShader->SetMat4("u_Transform", transform *  subtrans *subrot *rot);
			RenderCommand::DrawIndexedLineLoop(VertexArray::GetCapsuleVA(Shape.m_Size.x, Shape.m_Size.y));

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(Shape.m_Size.x));
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0, Shape.m_Size.y, 0));
			
			flatColorShader->SetMat4("u_Transform",  transform*subtrans*  subrot* trans   *scale);
			RenderCommand::DrawIndexedLineLoop(VertexArray::GetCircleVA());

			trans = glm::translate(glm::mat4(1.0f), glm::vec3(0, -Shape.m_Size.y, 0));
			flatColorShader->SetMat4("u_Transform", transform *subtrans   * subrot * trans * scale);
			RenderCommand::DrawIndexedLineLoop(VertexArray::GetCircleVA());
			
		}

		RenderCommand::SetDepthTest(true);
	}
}