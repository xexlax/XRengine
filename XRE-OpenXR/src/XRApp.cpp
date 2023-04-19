//#include <XRE.h>
//#include <xre/Core/entryPoint.h>
//#include <imgui.h>
//#include <Glad\glad.h>
//#include <GLFW\glfw3.h>
//
//#include "pch.h"
//#include "OpenXrProgram.h"
//
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
//constexpr const char* ProgramName = "BasicXrApp_win32";
//#else
//constexpr const char* ProgramName = "BasicXrApp_uwp";
//#endif
//
//using namespace XRE;
//class LauncherLayer : public XRE::Layer
//{
//
//	XRef<Project> m_Project;
//	XRef<Scene> m_Scene;
//	XRef<VertexArray> m_Quad;
//	XRef<Shader> shader;
//	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
//	glm::vec2 m_ViewportBounds[2];
//	unsigned int quadVAO, quadVBO;
//
//	uint32_t windowsSizeX = 1920, windowsSizeY = 1080;
//public:
//	LauncherLayer() {};
//	virtual ~LauncherLayer() = default;
//
//	virtual void OnAttach() override {
//
//		auto graphics = sample::CreateCubeGraphics();
//		auto program = sample::CreateOpenXrProgram(ProgramName, std::move(graphics));
//		program->Run();
//
//
//		m_Project = XMakeRef<Project>("..\\Projects\\project2\\project2.prj");
//		m_Project->Load();
//		ResourceManager::BindProj(m_Project);
//		Renderer3D::Init();
//		Renderer3D::m_FrameBuffer->Resize(windowsSizeX, windowsSizeY);
//		RenderCommand::SetViewport(0, 0, windowsSizeX, windowsSizeY);
//
//		shader = ResourceManager::GetShader("..\\Assets\\shaders\\texture.glsl");
//
//		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//			// positions   // texCoords
//			-1.0f,  1.0f,  0.0f, 1.0f,
//			-1.0f, -1.0f,  0.0f, 0.0f,
//			 1.0f, -1.0f,  1.0f, 0.0f,
//
//			-1.0f,  1.0f,  0.0f, 1.0f,
//			 1.0f, -1.0f,  1.0f, 0.0f,
//			 1.0f,  1.0f,  1.0f, 1.0f
//		};
//
//
//
//		glGenVertexArrays(1, &quadVAO);
//		glGenBuffers(1, &quadVBO);
//		glBindVertexArray(quadVAO);
//		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//
//		m_Scene = XMakeRef<Scene>();
//		
//		m_Scene->OnViewportResize(windowsSizeX, windowsSizeY);
//		m_Scene->OnRuntimeBegin();
//
//
//
//	};
//	virtual void OnDetach() override {
//		m_Scene->OnRuntimeEnd();
//		Renderer3D::ShutDown();
//	};
//
//	void OnUpdate(TimeStep ts) override {
//
//		Application::GetApplication().GetWindow().SetTitle(m_Project->m_Name + " FPS:" + std::to_string(Application::GetFPS()));
//
//		// make sure we clear the framebuffer's content
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		m_Scene->OnUpdateRuntime(ts);
//
//		shader->Bind();
//		glBindVertexArray(quadVAO);
//		glBindTexture(GL_TEXTURE_2D, Renderer3D::m_FrameBuffer->GetColorAttachment(0));	// use the color attachment texture as the texture of the quad plane
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//
//
//	}
//	bool OnWindowResize(WindowResizeEvent& e);
//	void OnEvent(XRE::Event& e)
//	{
//
//		EventDispatcher dispatcher(e);
//		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(LauncherLayer::OnWindowResize));
//
//
//	}
//
//
//
//
//};
//
//
//class LauncherApp : public XRE::Application
//{
//public:
//	LauncherApp();
//	~LauncherApp();
//
//private:
//
//};
//
//LauncherApp::LauncherApp()
//{
//	PushLayer(new LauncherLayer());
//}
//
//LauncherApp::~LauncherApp()
//{
//}
//
//XRE::Application* XRE::CreateApplication() {
//	return new LauncherApp();
//}
//
//bool LauncherLayer::OnWindowResize(WindowResizeEvent& e)
//{
//	Renderer3D::m_FrameBuffer->Resize(e.GetWidth(), e.GetHeight());
//	return false;
//
//}