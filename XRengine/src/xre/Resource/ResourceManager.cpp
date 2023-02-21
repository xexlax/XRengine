#include "pch.h"
#include "ResourceManager.h"

namespace XRE {

	Ref<Texture2DLibrary> ResourceManager::m_Tex2DLib = make_shared<Texture2DLibrary>();

	Ref<ShaderLibrary> ResourceManager::m_ShaderLib = make_shared<ShaderLibrary>();

	void ResourceManager::Init()
	{
		
	}
}