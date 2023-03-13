#include "pch.h"
#include "Material.h"
#include "ResourceManager.h"

namespace XRE {
	Material::Material()
	{
		name = "material";
		shininess = 64.0f;
		metallic = 0.5f, roughness = 0.5f;
		LoadAllTex();
	}
	Material::Material(std::string s)
	{
		name = "material";
		Load(s);
	}
	void Material::LoadAllTex()
	{
		if (diffuseTex.m_enable) 
			diffuseTex.m_Tex = ResourceManager::GetTex2D(diffuseTex);
		if (ambientTex.m_enable) 
			ambientTex.m_Tex = ResourceManager::GetTex2D(ambientTex);
		if (specularTex.m_enable) 
			specularTex.m_Tex = ResourceManager::GetTex2D(specularTex);
		if (specularHighlightTex.m_enable) 
			specularHighlightTex.m_Tex = ResourceManager::GetTex2D(specularHighlightTex);
		if (bumpTex.m_enable) 
			bumpTex.m_Tex = ResourceManager::GetTex2D(bumpTex);
		if (displacementTex.m_enable) 
			displacementTex.m_Tex = ResourceManager::GetTex2D(displacementTex);
		if (alphaTex.m_enable) 
			alphaTex.m_Tex = ResourceManager::GetTex2D(alphaTex);
		if (ambientOcclusionTex.m_enable)
			ambientOcclusionTex.m_Tex = ResourceManager::GetTex2D(alphaTex);
	}
	
	void Material::Save(const std::string& filepath)
	{
		m_FilePath = filepath;
		std::ofstream fs(filepath);
		if (fs.is_open()) {
			cereal::JSONOutputArchive ar(fs);
			ar(*this);
		}
		
	}
	void Material::Load(const std::string& filepath)
	{
		std::ifstream fs(filepath);
		if (fs.is_open()) {
			cereal::JSONInputArchive ar(fs);
			ar(*this);
		}
		
	}
	XRef<Material> Material::Create(const std::string& filepath)
	{
		return XMakeRef<Material>(filepath);
	}
}