#pragma once

#include "RendererAPI.h"

namespace XRE {

	//simple wrapper of RenderAPI funcs
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawIndexedLines(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexedLine(vertexArray);
		}

		inline static void DrawIndexedLineLoop(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexedLineLoop(vertexArray);
		}

		inline static void DrawSkyIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t cubemapTexture)
		{
			s_RendererAPI->DrawSkyIndexed(vertexArray,cubemapTexture);
		}
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void CullFace(bool b) {
			s_RendererAPI->CullFace(b);
		}

		inline static void SetDepthTest(bool b) {
			s_RendererAPI->SetDepthTest(b);
		}


	private:
		static RendererAPI* s_RendererAPI;
	};

}