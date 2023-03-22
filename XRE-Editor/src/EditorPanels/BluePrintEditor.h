#pragma once
#include "XRE.h"
namespace XRE {
	class BluePrintEditor {
		
	public:
		BluePrintEditor();
		
		void OnImGuiRender();

		void DrawNode(XRef<BluePrintNode> n);

		//当前的蓝图
		XRef<BluePrint> m_BluePrint;
		//当前蓝图的配套属性
		XRef<BluePrintProperties> m_Properties;
	private:
		int m_selectedNodes[16] = { -1 };

	};
}