
#include "pch.h"
#include "WindowsInput.h"

#include "xre\application.h"
#include "GLFW\glfw3.h"

#define XRE_GET_GLFW_WINDOW static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow())
namespace XRE {
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = XRE_GET_GLFW_WINDOW;
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = XRE_GET_GLFW_WINDOW;
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
		
	}
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = XRE_GET_GLFW_WINDOW;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}