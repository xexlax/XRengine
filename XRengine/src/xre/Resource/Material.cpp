#include "pch.h"
#include "Material.h"
#include "ResourceManager.h"

namespace XRE {
	Material::Material()
	{
		name = "default_material";
		shininess = 64.0f;
		metallic = 0.5f, roughness = 0.5f;
		LoadAllTex();
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
	}		
}