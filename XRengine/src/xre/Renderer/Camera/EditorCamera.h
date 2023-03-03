#pragma once
#include "pch.h"
#include "Camera.h"
#include "xre\Core\TimeStep.h"
#include "xre\Event\Event.h"
#include "xre\Event\MouseEvent.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace XRE {

	class EditorCamera : public Camera {
	public:
		EditorCamera();

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
		glm::quat GetOrientation() const;

		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		float m_Distance = 10.0f;
		bool m_FirstMouse = true;
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}