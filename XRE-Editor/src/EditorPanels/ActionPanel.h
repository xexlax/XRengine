#pragma once

namespace XRE {
	class ActionPanel {
	public:
		ActionPanel();

		void OnImGuiRender();
		bool m_updated = false;
	};
}