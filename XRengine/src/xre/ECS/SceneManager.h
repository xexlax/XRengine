#pragma once
#include "Scene.h"
namespace XRE {
	class SceneManager {

	public:
		static void SwitchScene(Scene* scene);

	private:

		static Scene* m_ActiveScene;
	};
}