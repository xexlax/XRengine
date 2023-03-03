#pragma once
#include "xre/Renderer/Resources/Texture.h"
#include "xre/Renderer/Resources/Shader.h"
#include "Model.h"
#include "Library.h"

namespace XRE {
	//��Դ�����������ȡ�����ʲ�������֤�䲻���ظ�����
	class ResourceManager {

	public:
		static void Init();
		
		static Ref<Shader> GetShader(const std::string& path) { return m_ShaderLib.Get(path); }
		static Ref<Texture2D> GetTex2D(const std::string& path) { return m_Texture2DLib.Get(path); }
		static Ref<Model> GetModel(const std::string& path) { return m_ModelLib.Get(path); }


	private:
		
		static Library<Shader> m_ShaderLib;
		static Library<Texture2D> m_Texture2DLib;
		static Library<Model> m_ModelLib;
		
	};
}