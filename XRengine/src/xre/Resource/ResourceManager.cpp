#include "pch.h"
#include "ResourceManager.h"


namespace XRE {

	Library<Shader> ResourceManager::m_ShaderLib;
	Library<Texture2D> ResourceManager::m_Texture2DLib;
	Library<Model> ResourceManager::m_ModelLib;
	Library<Material> ResourceManager::m_MaterialLib;
	Library<BluePrint> ResourceManager::m_BluePrintLib;
	Library<AudioClip> ResourceManager::m_AudioClipLib;
	XRef<Project> ResourceManager::m_CurProj;
	//Library<Prefab> ResourceManager::m_PrefabLib;

	void ResourceManager::Init()
	{
		m_CurProj = nullptr;
	}
	void ResourceManager::BindProj(XRef<Project> proj)
	{
		
		m_CurProj = proj;

		if (proj == nullptr) UnloadAllResources();
	}
	std::string ResourceManager::GetFullPath(std::string path)
	{
		if (path[0] == '.') return path;
		if (m_CurProj == nullptr) return  "..\\Assets\\"+path;

		else 
		return m_CurProj->m_RootPath + "\\Assets\\" + path;
	}
	std::string ResourceManager::GetRelativePath(std::string path)
	{
		if (m_CurProj == nullptr) return path;
		
		return path.substr((m_CurProj->m_RootPath + "\\Assets\\").length() );
	}
	void ResourceManager::UnloadAllResources()
	{
		m_ModelLib.UnloadAll();
		m_AudioClipLib.UnloadAll();
		m_BluePrintLib.UnloadAll();
		m_Texture2DLib.UnloadAll();
		m_ShaderLib.UnloadAll();
	}
	XRef<Texture2D> ResourceManager::GetNoTex2D() { return GetTex2D("textures\\No_Texture.png"); }
	XRef<Texture2D> ResourceManager::GetDefaultTex2D() { return GetTex2D("textures\\albedo.jpg"); }
	XRef<Model> ResourceManager::GetElementalModel(const Elemental_Model& em)
	{
		switch (em)
		{
		case Elemental_Model::Cube:
			return m_ModelLib.Get(GetFullPath("models\\cube.obj"));
		case Elemental_Model::Plane:
			return m_ModelLib.Get(GetFullPath("models\\plane.obj"));
		case Elemental_Model::Sphere:
			return m_ModelLib.Get(GetFullPath("models\\sphere.obj"));
		case Elemental_Model::Cone:
			return m_ModelLib.Get(GetFullPath("models\\cone.obj"));
		case Elemental_Model::Cylinder:
			return m_ModelLib.Get(GetFullPath("models\\cylinder.obj"));
		case Elemental_Model::Capsule:
			return m_ModelLib.Get(GetFullPath("models\\capsule.obj"));
		default:
			XRE_ERROR(u8"不存在的基础模型");
			break;
		}
	}
	XRef<Material> ResourceManager::GetDefaultMaterial() { return GetMaterial("..\\Assets\\materials\\default.mat"); }
}