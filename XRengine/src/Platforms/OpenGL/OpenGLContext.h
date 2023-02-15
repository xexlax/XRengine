#pragma once

#include "GLFW\glfw3.h"
#include "xre\Renderer\GraphicContext.h"

namespace XRE {
	class OpenGLContext: public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;

	};

	
}
