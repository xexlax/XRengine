#pragma once
#include "pch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace XRE {
	Camera::Camera(glm::mat4 P, glm::mat4 V ,glm::vec3 euler)
		: m_ProjectionMatrix(P), m_ViewMatrix(V),m_EulerAngles(euler)
	{

	}

	
	
	void PerspectiveCamera::RecalculateViewMatrix()
	{
		//m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0,0,0), m_Up);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		//m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0,0,0), m_Up);
		m_ViewMatrix = glm::mat3(glm::lookAt(m_Position, m_Position + m_Front, m_Up));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::updateCameraVectors()
	{
		//XRE_CORE_TRACE("front:{0} {1} {2}", m_Front.x, m_Front.y, m_Front.z);
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_EulerAngles.y)) * cos(glm::radians(m_EulerAngles.x));
		front.y = sin(glm::radians(m_EulerAngles.x));
		front.z = sin(glm::radians(m_EulerAngles.y)) * cos(glm::radians(m_EulerAngles.x));
		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		RecalculateViewMatrix();
	}


	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top,
		glm::vec3 position, glm::vec3 euler)
		: Camera(glm::ortho(left, right, bottom, top, 1.0f, 7.5f), glm::mat4(1.0f), euler)
	{
		m_Position = position;
		updateCameraVectors();
	}

	OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 target) :
		Camera(glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f),
			glm::mat4(1.0f))
	{
		m_WorldUp = glm::vec3(0, 1, 0);
		glm::vec3 front = target - position;
		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		RecalculateViewMatrix();

	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	
	PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float zNear, float zFar,
		glm::vec3 position, glm::vec3 euler):
		Camera(glm::perspective(fovy, aspect, zNear, zFar), glm::mat4(1.0f),euler)
	{
		m_Position = position;
		updateCameraVectors();
		
	}

	PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 target):
		Camera(glm::perspective(45.0f, 1.0f, 0.1f,100.0f), 
			glm::mat4(1.0f))
	{
		m_WorldUp = glm::vec3(0, 1, 0);
		glm::vec3 front = target-position;
		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		RecalculateViewMatrix();

	}

	void PerspectiveCamera::SetProjection(float fovy, float aspect, float zNear, float zFar)
	{
		m_ProjectionMatrix = glm::perspective(fovy,aspect,zNear,zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	
}