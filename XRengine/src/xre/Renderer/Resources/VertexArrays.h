#pragma once

#include "pch.h"
#include "xre\Renderer\Resources\Buffers.h"


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

	private:
		static XRef<VertexArray> CubeVA, CircleVA, SquareVA, CapsuleVA;
		static float Capsule_r, Capsule_l;
	};

	
}