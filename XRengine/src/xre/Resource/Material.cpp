#include "pch.h"
#include "Material.h"
#include "ResourceManager.h"

namespace XRE {
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