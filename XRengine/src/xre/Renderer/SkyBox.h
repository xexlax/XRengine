#pragma once
#include "xre\Renderer\Resources\Shader.h"
#include "xre\Core\Macros.h"
#include "xre\Renderer\Resources\VertexArrays.h"
namespace XRE {
	class SkyBox {
	public:

		SkyBox();
		~SkyBox();
		XRef<Shader> GetShader() const { return m_SkyBoxShader; };
		XRef<VertexArray> GetVAO() const { return m_VertexArray; };
		uint32_t GetCubemapTexture() const { return m_TextureID; };
		
	private:

		XRef<Shader> m_SkyBoxShader;
		uint32_t m_TextureID;
		XRef<VertexArray> m_VertexArray;

	};
}