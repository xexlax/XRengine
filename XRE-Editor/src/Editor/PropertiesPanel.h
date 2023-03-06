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

		//����go���е�Component
		void DrawComponents(GameObject go);
	private:

		ScenePanel* m_ReferenceScenePanel;


		//����Component ͨ��UI���֣�Treenode��active��ɾ���������
		template<typename T>
		void DrawComponent(GameObject go);

		//����Ϊ��ͬ��ľ��岼��
		template<typename T>
		void DrawComponentLayout(T& component);

	};
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue=0.0f, float columnWidth=110.0f);
}