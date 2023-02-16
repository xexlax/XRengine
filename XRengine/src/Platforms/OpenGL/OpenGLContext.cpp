#include "pch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace XRE {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):
		m_WindowHandle(windowHandle)
	{
		XRE_CORE_ASSERT(windowHandle, "Window Handle is Null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		XRE_CORE_ASSERT(status, "Failed to initialize Glad!");

		//XRE_CORE_INFO("OpenGL Info:");
		//XRE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//XRE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//XRE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
