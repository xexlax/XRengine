#pragma once
#include "../Commands/CommandManager.h"
namespace XRE::XUI{
	
		
		



	bool DragFloat(const std::string& label, float* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f" );
	bool CheckBox(const std::string& label, bool* value);
	
}