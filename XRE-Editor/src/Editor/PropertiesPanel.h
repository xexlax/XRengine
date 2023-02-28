#pragma once
#include <XRE.h>
#include "ScenePanel.h"
namespace XRE {
	class PropertiesPanel {
	public:
		PropertiesPanel()=default;
		PropertiesPanel(ScenePanel* sp);

		void SetReference(ScenePanel* sp);
		void OnImGuiRender();
	private:

		ScenePanel* m_ReferenceScenePanel;
	};
}