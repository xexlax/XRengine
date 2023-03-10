#pragma once

#include <glm/glm.hpp>

#include "Resources/VertexArrays.h"

namespace XRE {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawIndexedLine(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawIndexedLineLoop(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawSkyIndexed(const std::shared_ptr<VertexArray>& vertexArray,uint32_t cubemapTexture) = 0;
		virtual void CullFace(bool b)=0;
		virtual void SetDepthTest(bool b)= 0;
		
		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}