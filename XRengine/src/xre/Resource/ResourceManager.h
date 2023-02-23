#pragma once
#include "xre/Renderer/Resources/Texture.h"
#include "xre/Renderer/Resources/Shader.h"
#include "Model.h"

namespace XRE {
	class ResourceManager {

	public:
		static void Init();
		static Ref<Texture2DLibrary> GetTex2DLib() { return m_Tex2DLib; };
		static Ref<ShaderLibrary> GetShaderLib() { return m_ShaderLib; };

	private:
		static Ref<Texture2DLibrary> m_Tex2DLib;
		static Ref<ShaderLibrary> m_ShaderLib;
	};
}