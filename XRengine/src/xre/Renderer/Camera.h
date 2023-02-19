#pragma once
#include <glm\glm.hpp>
namespace XRE {
	class Camera {
	public:
		Camera(glm::mat4 P = glm::mat4(1.0f), glm::mat4 V = glm::mat4(1.0f));
		
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		virtual void SetRotation(float rotation)=0;
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		virtual void RecalculateViewMatrix()=0;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 front = { 0.0f, 0.0f, 1.0f };
	};

	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		float GetRotation() const { return m_Rotation;}
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		void SetProjection(float left, float right, float bottom, float top);
	protected:
		virtual void RecalculateViewMatrix();
		//1d rotation
		float m_Rotation = 0.0f;
	};

	class PerspectiveCamera : public Camera {
		PerspectiveCamera();
		glm::vec4 GetRotation() const { return m_Quaternion; }

	protected:
		virtual void RecalculateViewMatrix();
		//旋转四元数
		glm::vec4 m_Quaternion;
		glm::vec3 m_EulerAngles;
		
	};
}