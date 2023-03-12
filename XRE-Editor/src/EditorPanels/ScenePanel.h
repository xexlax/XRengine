#pragma once
#include <XRE.h>
namespace XRE {
	class ScenePanel {
		friend class PropertiesPanel;
	public:
		ScenePanel() = default;
		ScenePanel(const XRef<Scene>&scene);

		void AttachToScene(const XRef<Scene>&scene);
		void Select(int ObjId);
		void UnSelect();
		GameObject GetSelected() const { return m_Selected; }
		void OnImGuiRender();
	private:
		void DrawGONode(GameObject gameObj);
	private:
		XRef<Scene> m_Scene;
		GameObject m_Selected;
	};
}