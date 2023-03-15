#pragma once
#include "xre/Renderer/Resources/Texture.h"
#include "xre/Renderer/Resources/Shader.h"
#include "Model.h"
#include "Material.h"
#include "Library.h"

namespace XRE {
	//资源管理器负责获取各种资产，并保证其不被重复加载
	enum Elemental_Model {
		Cube,Plane,Sphere,Cone,Cylinder,Capsule
	};
	class ResourceManager {


	public:
		static void Init();
		
		static XRef<Shader> GetShader(const std::string& path) { return m_ShaderLib.Get(path); }
		
		
		static XRef<Texture2D> GetTex2D(const std::string& path) { return m_Texture2DLib.Get(path); }
		static XRef<Texture2D> GetNoTex2D() { return GetTex2D("../Assets/textures/No_Texture.png"); }
		
		
		static XRef<Model> GetModel(const std::string& path) { return m_ModelLib.Get(path); }
		static XRef<Model> GetElementalModel(const Elemental_Model& em);

		static XRef<Material> GetMaterial(const std::string& path) { return m_MaterialLib.Get(path); }
		static XRef<Material> GetDefaultMaterial() { return GetMaterial("../Assets/materials/default.mat"); }

	private:
		
		static Library<Shader> m_ShaderLib;
		static Library<Texture2D> m_Texture2DLib;
		static Library<Model> m_ModelLib;
		static Library<Material> m_MaterialLib;
		
	};
}