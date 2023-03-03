#include "pch.h"
#include "ResourceManager.h"

namespace XRE {

	Library<Shader> ResourceManager::m_ShaderLib;
	Library<Texture2D> ResourceManager::m_Texture2DLib;
	Library<Model> ResourceManager::m_ModelLib;

	void ResourceManager::Init()
	{
		
	}
}