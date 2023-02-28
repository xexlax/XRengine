#include "pch.h"
#include "CameraController.h"
#include "xre/Core/Input.h"
#include "xre/Core/KeyCodes.h"
#include "xre\Core\Macros.h"
#include <glm\ext\matrix_clip_space.hpp>


namespace XRE {
	CameraController::CameraController(float aspectRatio , glm::vec3 position)
		:m_AspectRatio(aspectRatio) ,m_Camera(glm::perspective(m_ZoomLevel,aspectRatio,0.1f,100.0f)),m_CameraPosition(position)
	{
		m_Camera.SetPosition(m_CameraPosition);
	}
	void CameraController::OnUpdate(XRE::TimeStep ts)
	{
		//XRE_CORE_INFO("CameraPos:{0} {1} {2}", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
		//XRE_CORE_INFO("CameraAngle:{0} {1} ", m_Camera.GetEuler().x, m_Camera.GetEuler().y);
		if (Input::IsKeyPressed(XRE_KEY_A))
			m_CameraPosition -= m_Camera.GetRight()* (m_CameraTranslationSpeed * ts);
		else if (Input::IsKeyPressed(XRE_KEY_D))
			m_CameraPosition += m_Camera.GetRight() * (m_CameraTranslationSpeed * ts);

		if (Input::IsKeyPressed(XRE_KEY_W))
			m_CameraPosition += m_Camera.GetFront() * (m_CameraTranslationSpeed * ts);
		else if (Input::IsKeyPressed(XRE_KEY_S))
			m_CameraPosition -= m_Camera.GetFront() * (m_CameraTranslationSpeed * ts);

		if (Input::IsKeyPressed(XRE_KEY_Q))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(XRE_KEY_E))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);

	}


	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CameraController::OnWindowResized));
		

	}
	void CameraController::Resize(float width, float height)
	{
		m_AspectRatio = width / height;

		m_Camera.SetProjection(glm::perspective(m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f));
	}
	bool CameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		float curX = e.GetX() , curY = e.GetY();
		if (firstMouse) {
			lastX = curX;
			lastY = curY;
			firstMouse = false;
		}

		float xoffset = e.GetX()-lastX;
		float yoffset = -e.GetY()+lastY;
		xoffset *= m_CameraSensitivity;
		yoffset *= m_CameraSensitivity;

		float Yaw = m_Camera.GetEuler().y;
		float Pitch = m_Camera.GetEuler().x;
		
		if (!Input::IsKeyPressed(XRE_KEY_LEFT_ALT)) {

			Yaw += xoffset;
			Pitch += yoffset;
		}
		
		

		lastX = curX;
		lastY = curY;

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
	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset()* m_CameraSensitivity;
		if (m_ZoomLevel < 1.0f) {
			m_ZoomLevel = 1.0f;
		}

		if (m_ZoomLevel > 45.0f) {
			m_ZoomLevel = 45.0f;
		}
		m_Camera.SetProjection(glm::perspective( m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f));
		return false;
	}
	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		////窗口最小化时需要防止除0出现NaN
		//if (e.GetHeight() == 0)return false;

		//m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		//m_Camera.SetProjection(m_ZoomLevel,m_AspectRatio,0.1f,100.0f);
		return false;
	}
	bool CameraController::OnKeyPressed(KeyPressedEvent& e)
	{
		
		return false;
	}
	bool CameraController::OnKeyReleased(KeyReleasedEvent& e)
	{
		
		return false;
	}
}