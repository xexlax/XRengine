#pragma once
#include "XRE.h"

namespace XRE {
	class MaterialPanel {
	public:
		MaterialPanel();

		void OnImGuiRender();

		void DrawTextureSlot(MaterialTex* tex);

		void Select(XRef<Material> s, MeshRendererComponent* c);
		void Switch();
	private:
		XRef<Material> m_Selected;
		MeshRendererComponent* comp;
		bool m_Switch;

	};
}