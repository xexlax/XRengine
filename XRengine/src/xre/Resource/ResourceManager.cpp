#include "pch.h"
#include "ResourceManager.h"


namespace XRE {

	Library<Shader> ResourceManager::m_ShaderLib;
	Library<Texture2D> ResourceManager::m_Texture2DLib;
	Library<Model> ResourceManager::m_ModelLib;
	Library<Material> ResourceManager::m_MaterialLib;
	Library<BluePrint> ResourceManager::m_BluePrintLib;
	XRef<Project> ResourceManager::m_CurProj;
	//Library<Prefab> ResourceManager::m_PrefabLib;

	void ResourceManager::Init()
	{
		m_CurProj = nullptr;
	}
	void ResourceManager::BindProj(XRef<Project> proj)
	{
		m_CurProj = proj;
	}
	std::string ResourceManager::GetFullPath(std::string path)
	{
		if (m_CurProj == nullptr) return  "..\\Assets\\"+path;

		else 
		return m_CurProj->m_RootPath + "\\Assets\\" + path;
	}
	std::string ResourceManager::GetRelativePath(std::string path)
	{
		if (m_CurProj == nullptr) return path;
		
		return path.substr(m_CurProj->m_RootPath.length());
	}
	XRef<Model> ResourceManager::GetElementalModel(const Elemental_Model& em)
	{
		switch (em)
		{
		case Elemental_Model::Cube:
			return m_ModelLib.Get("..\\Assets\\models\\cube.obj");
		case Elemental_Model::Plane:
			return m_ModelLib.Get("..\\Assets\\models\\plane.obj");
		case Elemental_Model::Sphere:
			return m_ModelLib.Get("..\\Assets\\models\\sphere.obj");
		case Elemental_Model::Cone:
			return m_ModelLib.Get("..\\Assets\\models\\cone.obj");
		case Elemental_Model::Cylinder:
			return m_ModelLib.Get("..\\Assets\\models\\cylinder.obj");
		case Elemental_Model::Capsule:
			return m_ModelLib.Get("..\\Assets\\models\\capsule.obj");
		default:
			XRE_ERROR(u8"不存在的基础模型");
			break;
		}
	}
}