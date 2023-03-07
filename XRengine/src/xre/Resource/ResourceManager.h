#pragma once
#include "xre/Renderer/Resources/Texture.h"
#include "xre/Renderer/Resources/Shader.h"
#include "Model.h"
#include "Library.h"

namespace XRE {
	//��Դ�����������ȡ�����ʲ�������֤�䲻���ظ�����
	enum Elemental_Model {
		Cube,Plane,Sphere,Cone,Cylinder,Torus
	};
	class ResourceManager {


	public:
		static void Init();
		
		static XRef<Shader> GetShader(const std::string& path) { return m_ShaderLib.Get(path); }
		static XRef<Texture2D> GetTex2D(const std::string& path) { return m_Texture2DLib.Get(path); }
		static XRef<Model> GetModel(const std::string& path) { return m_ModelLib.Get(path); }
		static XRef<Model> GetElementalModel(const Elemental_Model& em);


	private:
		
		static Library<Shader> m_ShaderLib;
		static Library<Texture2D> m_Texture2DLib;
		static Library<Model> m_ModelLib;
		
	};
}