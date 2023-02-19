#pragma once
#include "pch.h"
#include "xre\Core\Layer.h"

#include "xre/Event/AppEvent.h"
#include "xre/Event/KeyEvent.h"
#include "xre/Event/MouseEvent.h"

namespace XRE {
	class ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;

	};

	
}