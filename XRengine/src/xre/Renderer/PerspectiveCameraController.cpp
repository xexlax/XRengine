#include "pch.h"
#include "PerspectiveCameraController.h"
#include "xre/Core/Input.h"
#include "xre/Core/KeyCodes.h"
#include "xre\Core\Macros.h"

namespace XRE {
	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio)
		:m_AspectRatio(aspectRatio) ,m_Camera(m_ZoomLevel,aspectRatio,0.1f,100.0f)
	{

	}
	void PerspectiveCameraController::OnUpdate(XRE::TimeStep ts)
	{
		if (Input::IsKeyPressed(XRE_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(XRE_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(XRE_KEY_W))
			m_CameraPosition.z += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(XRE_KEY_S))
			m_CameraPosition.z -= m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(XRE_KEY_Q))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(XRE_KEY_E))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);

	}
	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}
	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		float curX = e.GetX() , curY = e.GetY();
		if (firstMouse) {
			lastX = curX;
			lastY = curY;
		}

		float xoffset = e.GetX()-lastX;
		float yoffset = e.GetY()-lastY;
		xoffset *= m_CameraSensitivity;
		yoffset *= m_CameraSensitivity;

		float Yaw = m_Camera.GetEuler().y;
		float Pitch = m_Camera.GetEuler().x;
		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (m_constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		m_Camera.SetRotation(glm::vec3(Pitch, Yaw, 0.0f));
		m_Camera.updateCameraVectors();
		return false;
	}
	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(m_ZoomLevel,m_AspectRatio,0.1f,100.0f);
		return false;
	}
}