#include <json11.hpp>
#include <fstream>
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"

namespace XRE {
	class SceneSerializer {

	public:
		SceneSerializer() = default;
		SceneSerializer(Scene* scene);

		void SaveToJsonFile(const std::string& path);
		void LoadFromJsonFile(const std::string& path);


	private:
		Scene* m_Scene = nullptr;
		
		json11::Json SerializeGameObj(GameObject go);
		
		GameObject DeserializeGameObj();


		template<typename T>
		json11::Json SerializeComponent(T* c);

		
		Component* DeserializeComponent();
	


		
	};
}