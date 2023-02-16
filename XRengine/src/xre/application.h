#pragma once
#include "xre/Macros.h"
#include "Event\AppEvent.h"
#include "LayerStack.h"
#include "ImGui\ImGuiLayer.h"
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
		inline Window& GetWindow() { return *m_Window; }
		static Application& GetApplication();
	private:
		//Singleton
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		LayerStack m_layerStack;

	};

	// To be defined in CLIENT
	Application* CreateApplication();
}