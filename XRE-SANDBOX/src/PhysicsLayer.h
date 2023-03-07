#pragma once
#include <XRE.h>



namespace XRE{

    class PhysicsLayer : public Layer {
	public:
		PhysicsLayer();
		virtual ~PhysicsLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(TimeStep ts) override;
		void OnEvent(Event & e) override;
		void OnImGuiRender() override;


		Ref<Scene> m_Scene;
		//Ref<PhysicsScene> m_PhysicsScene;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		EditorCamera m_EditorCamera;

    };
}