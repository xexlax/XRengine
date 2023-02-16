#include "pch.h"
#include "application.h"
#include "Event\AppEvent.h"

#include  "Input.h"

//Todo : 实现所有的RendererAPI，避免对glad在/platform外的引用 
namespace XRE {
	Application* Application::s_Instance = nullptr;
	
	Application::Application() {
	

		XRE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application() {

	}
	//通用回调函数，最终在glfwsetcallback中的lambda函数中被调用
	void Application::OnEvent(Event &e)
	{
		//构造dispatcher，并检验事件的类型执行对应的处理函数
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		//XRE_CORE_TRACE("{0}", e);

		//Layers自上而下处理事件
		for (auto it = m_layerStack.end();it != m_layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	

	void Application::Run() {
		
		
		while (m_running)
		{
			


			//layers 自下而上更新
			for (Layer* layer : m_layerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_layerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();


			//XRE_TRACE("{0},{1}", Input::GetMouseX(), Input::GetMouseY());

			m_Window->OnUpdate();
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return false;
	}
	Application& Application::GetApplication()
	{
		{ return *s_Instance; }
	}
}