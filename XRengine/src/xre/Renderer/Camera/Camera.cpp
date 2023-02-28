#pragma once
#include "pch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace XRE {
	Camera::Camera(CameraType type, glm::vec3 euler, glm::vec3 pos):
		m_EulerAngles(euler), m_Position(pos), m_Type(type)
	{
		RecalculateProjectionMatrix();
		updateCameraVectors();
		
	}
	Camera::Camera(glm::mat4 P, glm::mat4 V ,glm::vec3 euler,glm::vec3 pos)
		: m_ProjectionMatrix(P), m_ViewMatrix(V),m_EulerAngles(euler),m_Position(pos)
	{
		updateCameraVectors();
	}


	void Camera::updateCameraVectors()
	{
		//XRE_CORE_TRACE("front:{0} {1} {2}", m_Front.x, m_Front.y, m_Front.z);
		// calculate the new Front vector
		glm::vec3 front,right;

		front =glm::vec3(glm::vec4(0, 0, -1, 0)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngles.x), { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngles.y), { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngles.z), { 0, 0, 1 }));

		right = glm::vec3(glm::vec4(1, 0, 0, 0)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngles.x), { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngles.y), { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngles.z), { 0, 0, 1 }));

		//front.x = cos(glm::radians(m_EulerAngles.y)) * cos(glm::radians(m_EulerAngles.x));
		//front.y = sin(glm::radians(m_EulerAngles.x));
		//front.z = sin(glm::radians(m_EulerAngles.y)) * cos(glm::radians(m_EulerAngles.x));



		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(right);  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		

		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		RecalculateViewMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		switch (m_Type) {
		case Orthographic:
			m_ViewMatrix = glm::mat3(glm::lookAt(m_Position, m_Position + m_Front, m_Up));
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
			break;
		case Perspective:
			m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
			break;
		default:
			XRE_CORE_ERROR("错误的相机类型");
		}
	}

	void Camera::RecalculateProjectionMatrix()
	{
		switch (m_Type) {
		case Orthographic:
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;
			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
				m_OrthographicNear, m_OrthographicFar);
			
		}
		break;
		case Perspective:
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFovy, m_AspectRatio,m_PerspectiveNear, m_PerspectiveFar);
			break;
		default:
			XRE_CORE_ERROR("错误的相机类型");
		}
	}

	void Camera::SetViewPortSize(uint32_t width, uint32_t height)
	{
		if (height > 0.1f) {
			m_AspectRatio = (float)width / (float)height;
			RecalculateProjectionMatrix();
		}
		
	}

	void Camera::SetType(const CameraType& type)
	{
		m_Type = type;
		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}

	
}