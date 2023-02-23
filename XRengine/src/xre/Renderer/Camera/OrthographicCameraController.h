#pragma once


#include "Camera.h"
#include "xre/Core/Timestep.h"

#include "xre/Event/AppEvent.h"
#include "xre/Event/MouseEvent.h"

namespace XRE {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(XRE::TimeStep ts);
		void OnEvent(Event& e);

		Ref<OrthographicCamera> GetCamera() { return std::make_shared<OrthographicCamera>(m_Camera); }
		

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}