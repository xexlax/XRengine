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

	bool DragInt(const std::string& label, int* value, int speed, int min, int max, const char* format)
	{

		if (ImGui::DragInt(label.c_str(), value, speed, min, max, format)) {
			if (!CommandManager::Get().IsMakingCommand())
				CommandManager::Get().Begin_Command_Edit<int>(value, *value);
			return true;
		}
		else {
			if (CommandManager::Get().IsMakingCommand() && !Input::IsMouseButtonPressed(XRE_MOUSE_BUTTON_LEFT))
				CommandManager::Get().End_Command_Edit<int>(value, *value);
			return false;
		}

	}

	bool CheckBox(const std::string& label, bool* value) {

		if (ImGui::Checkbox(label.c_str(), value)) {
			
			CommandManager::Get().Command_Edit<bool>(value,!*value,*value);
			return true;
		}
		else {
			return false;
		}
	}
	bool InputText(const std::string& label,std::string* value)
	{
		std::string old_value = *value;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), value->c_str());
		if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer))) {
			*value = std::string(buffer);
			CommandManager::Get().Command_Edit<std::string>(value,old_value,*value);
			return true;
		}
		else {
			
			return false;
		}
	}
}