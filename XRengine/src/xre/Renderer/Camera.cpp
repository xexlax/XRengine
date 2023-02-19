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
	PerspectiveCamera::PerspectiveCamera()
	{
		
	}
	void PerspectiveCamera::RecalculateViewMatrix()
	{
	}
}