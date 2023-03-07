#include <XRE.h>
#include <xre/Core/entryPoint.h>
#include "ImGui\imgui.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsLayer.h"
//#include "Sandbox2D.h"
using namespace XRE;

//Experiment Layer
class EditorApp: public XRE::Application
{
public:
	EditorApp();
	~EditorApp();

private:

};

EditorApp::EditorApp()
{
	
	//PushLayer(new expLayer());

	//PushLayer(new Sandbox2D());

	PushLayer(new PhysicsLayer());
	
}

EditorApp::~EditorApp()
{
}

XRE::Application* XRE::CreateApplication() {
	return new EditorApp();
}
