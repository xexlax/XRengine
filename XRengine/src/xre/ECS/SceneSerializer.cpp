#include "pch.h"
#include <entt\entt.hpp>
#include "SceneSerializer.h"
using namespace entt::literals;
namespace XRE {
	SceneSerializer::SceneSerializer(Scene* scene)
	{
		m_Scene = scene;
	}
	void SceneSerializer::SaveToJsonFile(const std::string& path)
	{
		std::ofstream file(path);
		if (file.is_open()) {

			std::vector<json11::Json> objContexts;


			m_Scene->m_Registry.each([&](auto entityID)
				{
					//shared_ptr.get 返回其raw pointer
			
				});
		}
		else {
			XRE_CORE_ERROR("无法将场景写入文件：{0}", path);
		}


	}
	void SceneSerializer::LoadFromJsonFile(const std::string& path)
	{
	}

	template <typename T>
	json11::Json SceneSerializer::SerializeComponent(T* c)
	{
		json11::Json compjs;

		for (auto [id, type] : entt::resolve<T>().data()) {
			json11::Json {id, c.type }
		}

		return ;
	}

	Component* SceneSerializer::DeserializeComponent()
	{
		return nullptr;
	}

	
	

	
}