#pragma once
#include "pch.h"
#include "xre/Core/Macros.h"
#include <glm/glm.hpp>

namespace XRE {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode); 
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}