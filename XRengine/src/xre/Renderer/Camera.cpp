#pragma once
#include "pch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace XRE {
	Camera::Camera(glm::mat4 P, glm::mat4 V )
		: m_ProjectionMatrix(P), m_ViewMatrix(V)
	{

	}
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f),glm::mat4(1.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	//To Impl
	
	PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float zNear, float zFar,
		glm::vec3 position, glm::vec3 up, glm::vec3 euler)
		:m_Position(position), m_EulerAngles(euler), m_WorldUp(up), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		Camera(glm::perspective(fovy, aspect, zNear, zFar), glm::mat4(1.0f))
	{
		updateCameraVectors();
		RecalculateViewMatrix();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float fovy, float aspect, float zNear, float zFar)
	{
		m_ProjectionMatrix = glm::perspective(fovy,aspect,zNear,zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_EulerAngles.y)) * cos(glm::radians(m_EulerAngles.x));
		front.y = sin(glm::radians(m_EulerAngles.x));
		front.z = sin(glm::radians(m_EulerAngles.y)) * cos(glm::radians(m_EulerAngles.x));
		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
}