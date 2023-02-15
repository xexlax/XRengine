#include <XRE.h>
#include "ImGui\imgui.h"

class expLayer : public XRE::Layer {
public:
	expLayer() :Layer("example") {

	}

	void OnUpdate() override
	{
		//XRE_INFO("ExampleLayer::Update");
		if (XRE::Input::IsKeyPressed(XRE_KEY_TAB))
			XRE_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(XRE::Event& event) override
	{
		if (event.GetEventType() == XRE::EventType::KeyPressed)
		{
			XRE::KeyPressedEvent& e = (XRE::KeyPressedEvent&)event;
			if (e.GetKeyCode() == XRE_KEY_TAB)
				XRE_TRACE("Tab key is pressed (event)!");
			XRE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};
class SandBox: public XRE::Application
{
public:
	SandBox();
	~SandBox();

private:

};

SandBox::SandBox()
{
	PushLayer(new expLayer());
	
}

SandBox::~SandBox()
{
}

XRE::Application* XRE::CreateApplication() {
	return new SandBox();
}
