#pragma once
#include "XRE.h"
namespace XRE {
	class BluePrintEditor {
		
	public:
		BluePrintEditor();
		
		void OnImGuiRender();

		void DrawNode(XRef<BluePrintNode> n);

	
		XRef<BluePrint> m_BluePrint;
	private:
		int m_selectedNodes[16] = { -1 };

	};
}