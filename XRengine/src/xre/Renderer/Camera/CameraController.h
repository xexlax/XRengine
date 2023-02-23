#pragma once
#include <xre\Core\TimeStep.h>
#include <xre\Event\Event.h>
#include <xre\Renderer\Camera\Camera.h>
#include <xre\Event\MouseEvent.h>
#include <xre\Event\AppEvent.h>

namespace XRE {
	class CameraController {

		virtual void OnUpdate(TimeStep ts)=0;
		virtual void OnEvent(Event& e)=0;
		Ref<Camera> GetCamera() { return std::make_shared<Camera>(m_Camera); }

		virtual bool OnMouseScrolled(MouseScrolledEvent& e)=0;
		virtual bool OnWindowResized(WindowResizeEvent& e)=0;
	protected:
		Ref<Camera> m_Camera;

	};
}