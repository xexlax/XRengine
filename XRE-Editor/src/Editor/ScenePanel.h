#pragma once
#include <XRE.h>
namespace XRE {
	class ScenePanel {
		friend class PropertiesPanel;
	public:
		ScenePanel() = default;
		ScenePanel(const Ref<Scene>&scene);

		void AttachToScene(const Ref<Scene>&scene);
		GameObject GetSelected() const { return m_Selected; }
		void OnImGuiRender();
	private:
		void DrawGONode(GameObject gameObj);
	private:
		Ref<Scene> m_Scene;
		GameObject m_Selected;
	};
}