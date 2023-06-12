#pragma once

#include "pch.h"
#include "xre\Renderer\Resources\Buffers.h"
#include "glm/glm.hpp"


namespace XRE {
	class VertexArray
	{
	public:
		
		~VertexArray() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static XRef<VertexArray> Create();

		static XRef<VertexArray> GetCubeVA();
		static XRef<VertexArray> GetCircleVA();
		static XRef<VertexArray> GetSquareVA();
		static XRef<VertexArray> GetCapsuleVA(float r, float l);
		static XRef<VertexArray> GetSpriteVA(float w, float h);

		static XRef<VertexArray> GetLinesVA(std::vector<glm::vec3>& points);
		static XRef<VertexArray> GetRayVA(glm::vec3 origin, glm::vec3 dir, float length = 100.0f);

	private:
		static XRef<VertexArray> CubeVA, CircleVA, SquareVA, CapsuleVA;
		static float Capsule_r, Capsule_l;
	};

	
}