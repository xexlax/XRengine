#pragma once
#include <XRE.h>

namespace XRE {
	class PropertiesPanel {
	public:
		PropertiesPanel()=default;

		void SetEC(EditorCamera* ec) {
			m_EditorCamera = ec;
		}
		
		void OnImGuiRender();

		//����go���е�Component
		void DrawComponents(GameObject go);
		void Switch() {
			m_Switch = true;
		}
	private:

		ScenePanel* m_ReferenceScenePanel;
		EditorCamera* m_EditorCamera=nullptr;


		//����Component ͨ��UI���֣�Treenode��active��ɾ���������
		template<typename T>
		void DrawComponent(GameObject go);

		//����Ϊ��ͬ��ľ��岼��
		template<typename T>
		void DrawComponentLayout(T& component);

		Component* m_Copied = nullptr;
		bool m_Switch = false;

		template<>
		void DrawComponentLayout(BluePrintComponent& component);

	};
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue=0.0f, float columnWidth=110.0f);
}