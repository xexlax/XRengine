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
			ResourceManager::GetTex2DLib()->Load(diffuseTex);
		if (ambientTex.m_enable) 
			ResourceManager::GetTex2DLib()->Load(ambientTex);
		if (specularTex.m_enable) 
			ResourceManager::GetTex2DLib()->Load(specularTex);
		if (specularHighlightTex.m_enable) 
			ResourceManager::GetTex2DLib()->Load(specularHighlightTex);
		if (bumpTex.m_enable) 
			ResourceManager::GetTex2DLib()->Load(bumpTex);
		if (displacementTex.m_enable) 
			ResourceManager::GetTex2DLib()->Load(displacementTex);
		if (alphaTex.m_enable) 
			ResourceManager::GetTex2DLib()->Load(alphaTex);
	}		
}