#include <XRE.h>
#include <xre/Core/entryPoint.h>
#include "ImGui\imgui.h"

#include "Platforms\OpenGL\OpenGLShader.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox2D.h"
using namespace XRE;

//Experiment Layer
class expLayer : public XRE::Layer {
public:
	expLayer() 
		: Layer("Example"), m_CameraController(1280.0f / 720.0f){
		
		//Renderable Object Setup Sample
		//1.Data and Structure
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		uint32_t indices[3] = { 0, 1, 2 };

		BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
		};

		//2.setup VAO
		m_VertexArray=VertexArray::Create();

		//2.1 setup VBO
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		//end


		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };


		m_SquareVA=VertexArray::Create();
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);


		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		//Init Shaders
		
		m_Shader = Shader::Create("assets/shaders/default.glsl");
		m_FlatColorShader = Shader::Create("assets/shaders/flatColor.glsl");
		auto textureShader= m_ShaderLibrary.Load("assets/shaders/texture.glsl");

		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_alpha_Texture = Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(textureShader)->setInt("u_Texture", 0);

	}

	void OnUpdate(XRE::TimeStep ts) override
	{
		m_CameraController.OnUpdate(ts);

		XRE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		XRE::RenderCommand::Clear();

		
		Renderer::BeginScene(m_CameraController.GetCamera());


		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->setFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				XRE::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
		XRE::Renderer::Submit(m_Shader, m_VertexArray);

		auto textureShader = m_ShaderLibrary.Get("texture");
		//glActiveTexture(GL_TEXTURE0);
		m_Texture->Bind();
		Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_alpha_Texture->Bind();
		Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		XRE::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(XRE::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}
private:

	

	Ref<Shader> m_Shader;

	ShaderLibrary m_ShaderLibrary;
	Ref<VertexArray> m_VertexArray;


	Ref<Shader> m_FlatColorShader;
	Ref<Texture2D> m_Texture,m_alpha_Texture;
	Ref<VertexArray> m_SquareVA;
	OrthographicCameraController m_CameraController;


	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};
class SandBox: public XRE::Application
{
public:
	SandBox();
	~SandBox();

private:

};

SandBox::SandBox()
{
	
	//PushLayer(new expLayer());

	PushLayer(new Sandbox2D());
	
}

SandBox::~SandBox()
{
}

XRE::Application* XRE::CreateApplication() {
	return new SandBox();
}
