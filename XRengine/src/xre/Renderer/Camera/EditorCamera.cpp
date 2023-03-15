#include "pch.h"

#include "EditorCamera.h"
#include "xre\Core\Input.h"
#include "xre\Core\KeyCodes.h"
namespace XRE {
	EditorCamera::EditorCamera():
		Camera(CameraType::Perspective)
	{
		m_PerspectiveFar = 1000.0f;
	}
	void EditorCamera::OnUpdate(TimeStep ts)
	{
		if (Input::IsMouseButtonPressed(XRE_MOUSE_BUTTON_RIGHT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (m_FirstMouse) m_FirstMouse = false;
			else {
				if (Input::IsKeyPressed(XRE_KEY_LEFT_ALT)) {
					MousePan(delta);
				}

				else if (Input::IsKeyPressed(XRE_KEY_LEFT_SHIFT))
				{
					MouseZoom(delta.y);
					
				}
				else
					MouseRotate(delta);
			}
			
		}
		else {
			m_FirstMouse = true;
		}
		updateCameraVectors();
		SetPosition(m_FocalPoint - GetFront() * m_Distance);
		
		
	}
	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}
	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		RecalculateViewMatrix();
		return false;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.5f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_EulerAngles.y, -m_EulerAngles.x, 0.0f));
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRight() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUp() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUp().y < 0 ? -1.0f : 1.0f;
		m_EulerAngles.y += glm::degrees( yawSign * delta.x * RotationSpeed());
		m_EulerAngles.x += glm::degrees(delta.y * RotationSpeed());
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetFront();
			m_Distance = 1.0f;
		}
	}
}