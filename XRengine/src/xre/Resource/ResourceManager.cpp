#include "pch.h"
#include "ResourceManager.h"

namespace XRE {

	Library<Shader> ResourceManager::m_ShaderLib;
	Library<Texture2D> ResourceManager::m_Texture2DLib;
	Library<Model> ResourceManager::m_ModelLib;

	void ResourceManager::Init()
	{
		
	}
	Ref<Model> ResourceManager::GetElementalModel(const Elemental_Model& em)
	{
		switch (em)
		{
		case Elemental_Model::Cube:
			return m_ModelLib.Get("../Assets/models/cube.obj");
		case Elemental_Model::Plane:
			return m_ModelLib.Get("../Assets/models/plane.obj");
		case Elemental_Model::Sphere:
			return m_ModelLib.Get("../Assets/models/sphere.obj");
		case Elemental_Model::Cone:
			return m_ModelLib.Get("../Assets/models/cone.obj");
		case Elemental_Model::Cylinder:
			return m_ModelLib.Get("../Assets/models/cylinder.obj");
		case Elemental_Model::Torus:
			return m_ModelLib.Get("../Assets/models/torus.obj");
		default:
			XRE_ERROR("不存在的基本模型种类");
			break;
		}
	}
}