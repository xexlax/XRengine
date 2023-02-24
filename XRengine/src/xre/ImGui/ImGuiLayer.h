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
		virtual void OnEvent(Event &e) override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetStyle();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

	};

	
}