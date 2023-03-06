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

		//绘制go所有的Component
		void DrawComponents(GameObject go);
	private:

		ScenePanel* m_ReferenceScenePanel;


		//绘制Component 通用UI部分（Treenode，active，删除等组件）
		template<typename T>
		void DrawComponent(GameObject go);

		//重载为不同类的具体布局
		template<typename T>
		void DrawComponentLayout(T& component);

	};
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue=0.0f, float columnWidth=110.0f);
}