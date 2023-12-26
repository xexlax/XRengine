#pragma once
#include "xre/Renderer/Resources/Texture.h"
#include "xre/Renderer/Resources/Shader.h"
#include "xre/BluePrint/BluePrint.h"
#include "Model.h"
#include "Material.h"
#include "Library.h"
#include "xre/Project/project.h"
#include "xre\Audio\Audio.h"
//#include "Prefab.h"

namespace XRE {
	//资源管理器，通过路径获取资源，确保资源不会重复加载
	enum Elemental_Model {
		Cube, Plane, Sphere, Cone, Cylinder, Capsule
	};
	class ResourceManager {


	public:
		static void Init();
		static void BindProj(XRef<Project> proj);
		static std::string GetFullPath(std::string path);
		static std::string GetRelativePath(std::string path);


		static void UnloadAllResources();



		static XRef<AudioClip> GetAudioClip(const std::string& path) { return m_AudioClipLib.Get(GetFullPath(path)); }
		static XRef<AudioClip> GetEditorAudioClip(const std::string& path) { return m_AudioClipLib.Get(path); }

		static XRef<Shader> GetShader(const std::string& path) { return m_ShaderLib.Get(path); }
		static XRef<Texture2D> GetTex2D(const std::string& path) { return m_Texture2DLib.Get(GetFullPath(path)); }
		static XRef<Texture2D> GetEditorTex2D(const std::string& path) { return m_Texture2DLib.Get(path); }
		static XRef<Texture2D> GetNoTex2D();
		static XRef<Texture2D> GetDefaultTex2D();

		static XRef<Model> GetModel(const std::string& path) { return m_ModelLib.Get(GetFullPath(path)); }
		static XRef<Model> GetElementalModel(const Elemental_Model& em);

		static XRef<Material> GetMaterial(const std::string& path) { return m_MaterialLib.Get(GetFullPath(path)); }
		static XRef<Material> GetDefaultMaterial();

		static XRef<BluePrint> GetBluePrint(const std::string& path) { return  m_BluePrintLib.Get(GetFullPath(path)); }


		
	private:

		static Library<Shader> m_ShaderLib;
		static Library<Texture2D> m_Texture2DLib;
		static Library<Model> m_ModelLib;
		static Library<Material> m_MaterialLib;
		static Library<BluePrint> m_BluePrintLib;
		static Library<AudioClip> m_AudioClipLib;
		static XRef<Project> m_CurProj;
		//static Library<Prefab> m_PrefabLib; // 添加Prefab库

	};
}