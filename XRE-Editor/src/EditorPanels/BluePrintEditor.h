#pragma once
#include "XRE.h"

namespace XRE {
	class BluePrintEditor {
		
	public:
		BluePrintEditor();
		
		void OnImGuiRender();

		void DrawNode(XRef<BluePrintNode> n);

		void Switch();
		std::string m_Info= u8"Global";
		//当前的蓝图
		XRef<BluePrint> m_BluePrint;
		//当前蓝图的配套属性
		XRef<BluePrintProperties> m_Properties;
	private:
		template <typename T>
		void AddNode() {
			XRef<BluePrintNode> n = m_BluePrint->MakeNode<T>();
			//ImNodes::SetNodeScreenSpacePos(n->NodeID, ImGui::GetMousePosOnOpeningCurrentPopup());
		}
		void SaveNodesLayout();
		int m_selectedNodes[16] = { -1 };
		bool m_switch = false;
	};
}