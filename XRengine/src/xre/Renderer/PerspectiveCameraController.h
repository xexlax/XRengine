#pragma once


#include "xre/Renderer/Camera.h"
#include "xre/Core/Timestep.h"

#include "xre/Event/AppEvent.h"
#include "xre/Event/MouseEvent.h"

namespace XRE {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float aspectRatio,glm::vec3 position=glm::vec3(0.0f,0.0f,-3.0f));

		void OnUpdate(XRE::TimeStep ts);
		void OnEvent(Event& e);

		Ref<PerspectiveCamera> GetCamera() { return std::make_shared<PerspectiveCamera>(m_Camera); }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:

		float m_AspectRatio;
		float m_ZoomLevel = 45.0f;

		float lastX=0, lastY=0;
		bool firstMouse = true;
		PerspectiveCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 3.0f };

		bool m_constrainPitch = true;
		
		float m_CameraTranslationSpeed = 5.0f,m_CameraSensitivity=0.2f;
	};

}