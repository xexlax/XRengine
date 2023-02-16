#include <XRE.h>
#include "ImGui\imgui.h"
using namespace XRE;

//Experiment Layer
class expLayer : public XRE::Layer {
public:
	expLayer() :Layer("example") {
		m_Camera.reset<OrthographicCamera>(new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));

		//Renderable Object Setup Sample
		//1.Data and Structure
		float vertices[4 * 7] = {
			-0.5f, 0.0f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.5f, 0.0f, 0.0f, 0.8f, 0.4f, 0.6f, 1.0f,
			 0.0f,  0.75f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
			 0.0f,  -0.75f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f,
		};
		uint32_t indices[3 * 2] = { 0, 1, 2 ,0, 3, 1 };

		BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
		};

		//2.setup VAO
		m_VertexArray.reset(VertexArray::Create());

		//2.1 setup VBO
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		//end


		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };


		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);


		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		//Init Shaders
		m_Shader.reset(new Shader("./shaders/default.vs", "./shaders/default.fs"));
		m_BlueShader.reset(new Shader("./shaders/blue.vs", "./shaders/blue.fs"));
	}

	void OnUpdate() override
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_Camera->SetPosition({ 0.5f, 0.5f, 0.0f });

		m_Camera->SetRotation(45.0f);
		Renderer::BeginScene(m_Camera);


		Renderer::Submit(m_BlueShader, m_SquareVA);

		Renderer::Submit(m_Shader, m_VertexArray);
	}

	virtual void OnImGuiRender() override {
		
	}

	void OnEvent(XRE::Event& event) override
	{
		if (event.GetEventType() == XRE::EventType::KeyPressed)
		{
			XRE::KeyPressedEvent& e = (XRE::KeyPressedEvent&)event;
			if (e.GetKeyCode() == XRE_KEY_TAB)
				XRE_TRACE("Tab key is pressed (event)!");
			XRE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
private:

	std::shared_ptr<Camera> m_Camera;

	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;


	std::shared_ptr<Shader> m_BlueShader;
	std::shared_ptr<VertexArray> m_SquareVA;
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
	
	PushLayer(new expLayer());
	
}

SandBox::~SandBox()
{
}

XRE::Application* XRE::CreateApplication() {
	return new SandBox();
}
