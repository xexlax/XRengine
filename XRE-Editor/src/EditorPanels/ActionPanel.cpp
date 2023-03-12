
#include "ActionPanel.h"
#include "../Commands/CommandManager.h"
#include <ImGui\imgui.h>
namespace XRE {
	ActionPanel::ActionPanel()
	{
	}
	void ActionPanel::OnImGuiRender()
	{
		ImGui::Begin(u8"¶¯×÷");
		{
			
			
			for (auto command : CommandManager::Get().UndoStack) {
				ImGui::Button(command->GetString().c_str());
			}
			ImGui::BeginDisabled();
			for (auto command : CommandManager::Get().RedoStack) {
				ImGui::Button(command->GetString().c_str());
			}
			ImGui::EndDisabled();

			if (m_updated) {
				m_updated = false;
				ImGui::SetScrollHereY(1.0f);
			}
			
		}
		ImGui::End();
	}
}