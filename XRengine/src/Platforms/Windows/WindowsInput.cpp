
#include "pch.h"
#include "xre\Core\Input.h"

#include "xre\Core\application.h"
#include "GLFW\glfw3.h"

#define XRE_GET_GLFW_WINDOW static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow())
namespace XRE {
	

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = XRE_GET_GLFW_WINDOW;
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = XRE_GET_GLFW_WINDOW;
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
		
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = XRE_GET_GLFW_WINDOW;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}