#pragma once
#include <glm\glm.hpp>
namespace XRE {
	class Camera {
	public:
		Camera(glm::mat4 P = glm::mat4(1.0f), glm::mat4 V = glm::mat4(1.0f), glm::vec3 euler = glm::vec3(0.0f, -90.0f, 0.0f));
		
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(glm::vec3 euler) { m_EulerAngles = euler; RecalculateViewMatrix(); }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		glm::vec3 GetEuler() const { return m_EulerAngles; }
		glm::vec3 GetFront() const { return m_Front; }
		glm::vec3 GetRight() const { return m_Right; }

		void updateCameraVectors();
		virtual void RecalculateViewMatrix() = 0;
	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	public:
		glm::vec3 m_EulerAngles;
		glm::vec3 m_Front, m_Up, m_Right;
		glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

	};

	class OrthographicCamera : public Camera {
	public:
		
		OrthographicCamera(
			glm::vec3 position,
			glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f));


		OrthographicCamera(float left, float right, float bottom, float top,
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3 euler = glm::vec3(0.0f, -90.0f, 0.0f));

		void SetProjection(float fovy, float aspect, float znear, float zfar);
	
		void RecalculateViewMatrix() override;
		
	};

	class PerspectiveCamera : public Camera {

		
	public:
		PerspectiveCamera(
			float fovy, float aspect, float zNear, float zFar,
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
			glm::vec3 euler =    glm::vec3(0.0f, -90.0f,0.0f) );

		PerspectiveCamera(
			glm::vec3 position,
			glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f));

		void SetProjection(float fovy, float aspect, float znear, float zfar);
		void RecalculateViewMatrix() override;
		
	};
}