#include "pch.h"
#include "application.h"
#include "xre\Event\AppEvent.h"
#include "xre\Renderer\Renderer.h"
#include "xre\Resource\ResourceManager.h"
#include "xre\ECS\Reflection.h"
#include "xre\Audio\alManager.h"
#include  "Input.h"
#include <GLFW\glfw3.h>

#ifdef XRE_RENDERER_VULKAN
	#include "Platforms\Vulkan\VkContext.h"
#endif

#include <thread>

//Todo : ʵ�����е�RendererAPI�������glad��/platform������� 
namespace XRE {
	int Application::m_FPS;
	Application* Application::s_Instance = nullptr;
	
	Application::Application() {
	

		XRE_CORE_ASSERT(!s_Instance, "�Ѵ���������Ӧ��ʵ��");
		s_Instance = this;
		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		ResourceManager::Init();
		Reflection::Init();
		alManager::Init();


		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_LastSecond = (float)glfwGetTime();
		m_FPS = 0;

	}

	Application::~Application() {
		ResourceManager::UnloadAllResources();
		alManager::End();
	}
	//ͨ�ûص�������������glfwsetcallback�е�lambda�����б�����
	void Application::OnEvent(Event &e)
	{
		//����dispatcher���������¼�������ִ�ж�Ӧ�Ĵ�����
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		//XRE_CORE_TRACE("{0}", e);

		//Layers���϶��´����¼�
		for (auto it = m_layerStack.end();it != m_layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	

	void Application::Run() {
		
		
		while (m_running)
		{
			
			float time = (float)glfwGetTime();
			TimeStep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			//FPS_COUNTER

			if (time - m_LastSecond >= 1) {
				m_LastSecond = time;
				m_FPS = m_FrameCount;
				m_FrameCount = 0;
			}
			m_FrameCount++;
			if (!m_Minimized) {
#ifdef XRE_RENDERER_VULKAN

				VkContext::GetInstance()->beginFrame();
#endif


				//layers ���¶��ϸ���

				{
					for (Layer* layer : m_layerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();

				for (Layer* layer : m_layerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();

#ifdef XRE_RENDERER_VULKAN

				VkContext::GetInstance()->endFrame();
#endif
				//XRE_TRACE("{0},{1}", Input::GetMouseX(), Input::GetMouseY());
			}
			m_Window->OnUpdate();


		}
	}
	void Application::Close() {
		
		m_running = false;
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
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
	Application& Application::GetApplication()
	{
		{ return *s_Instance; }
	}
}