#pragma once
#include "xre\Core\Macros.h"
#include "xre\Event\AppEvent.h"
#include "LayerStack.h"
#include "xre\ImGui\ImGuiLayer.h"
#include "Window.h"

namespace XRE {
	class Application{
	public:
		Application();
		virtual ~Application();
		void OnEvent(Event& e);
		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		inline Window& GetWindow() { return *m_Window; }
		static Application& GetApplication();
	private:
		//Singleton
		static Application* s_Instance;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		
		bool m_Minimized = false;
		LayerStack m_layerStack;
		float m_LastFrameTime = 0.0f;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}