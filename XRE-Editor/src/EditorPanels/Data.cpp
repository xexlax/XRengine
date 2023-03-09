#include "Data.h"

#include <ImGui\imgui.h>
#include <XRE.h>

namespace XRE::XUI{
	bool DragFloat(const std::string& label, float* value, float speed, float min, float max, const char* format)
	{

		if (ImGui::DragFloat(label.c_str(), value, speed, min, max,format)) {
			if (!CommandManager::Get().IsMakingCommand())
				CommandManager::Get().Begin_Command_Edit<float>(value, *value);
			return true;
		}
		else {
			if (CommandManager::Get().IsMakingCommand()&&!Input::IsMouseButtonPressed(XRE_MOUSE_BUTTON_LEFT))
				CommandManager::Get().End_Command_Edit<float>(value, *value);
			return false;
		}

	}

	bool CheckBox(const std::string& label, bool* value) {

		if (ImGui::Checkbox(label.c_str(), value)) {
			if (!CommandManager::Get().IsMakingCommand())
				CommandManager::Get().Begin_Command_Edit<bool>(value, *value);
			return true;
		}
		else {
			if (CommandManager::Get().IsMakingCommand() && !Input::IsMouseButtonPressed(XRE_MOUSE_BUTTON_LEFT))
				CommandManager::Get().End_Command_Edit<bool>(value, *value);
			return false;
		}
	}
}