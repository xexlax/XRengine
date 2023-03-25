#pragma once

#include "xre/Renderer/RendererAPI.h"

namespace XRE {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void Init() override;

		virtual void CullFace(bool b) override;
		virtual void SetDepthTest(bool b) override;
		
	
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawIndexedLine(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawIndexedLineLoop(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawSkyIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t cubemapTexture);
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};


}