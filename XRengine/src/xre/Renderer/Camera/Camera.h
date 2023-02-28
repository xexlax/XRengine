#pragma once
#include <glm\glm.hpp>
namespace XRE {
	enum  CameraType
	{
		Orthographic=0, Perspective
	};
	class Camera {
	public:

		Camera(CameraType type, glm::vec3 euler = glm::vec3(0.0f, -90.0f, 0.0f),
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));

		Camera(glm::mat4 P = glm::mat4(1.0f), glm::mat4 V = glm::mat4(1.0f),
			glm::vec3 euler = glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
		
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(glm::vec3 euler) { m_EulerAngles = euler; RecalculateViewMatrix(); }
		void SetProjection(glm::mat4 proj) { m_ProjectionMatrix = proj; RecalculateViewMatrix(); };
		void SetType(const CameraType& type);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		glm::vec3 GetEuler() const { return m_EulerAngles; }
		glm::vec3 GetFront() const { return m_Front; }
		glm::vec3 GetRight() const { return m_Right; }

		void updateCameraVectors();
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
		void SetViewPortSize(uint32_t width, uint32_t height);
	protected:
		


		CameraType m_Type = Perspective;

		//属性
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_EulerAngles;

		//矩阵
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		

		//方向向量
		
		glm::vec3 m_Front, m_Up, m_Right;
		glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

		//参数
		float m_OrthographicSize=20.0f, m_OrthographicNear=-10.0f, m_OrthographicFar=10.0f;
		float m_PerspectiveFovy=45.0f, m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;

		float m_AspectRatio=1.0f;
		
		

	};

	

	
}