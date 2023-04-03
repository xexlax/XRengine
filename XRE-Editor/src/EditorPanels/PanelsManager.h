#pragma once
#include "ActionPanel.h"
#include "AssetsPanel.h"
#include "PropertiesPanel.h"
#include "ScenePanel.h"
#include "MaterialPanel.h"
#include "BluePrintEditor.h"

namespace XRE{
	class PanelsManager {

		static XRef<PanelsManager> instance;

		ScenePanel m_ScenePanel;
		ActionPanel m_ActionPanel;
		AssetsPanel m_AssetsPanel;
		PropertiesPanel m_PropertiesPanel;
		MaterialPanel m_MaterialPanel;
		BluePrintEditor m_BluePrintEditor;


	public:
		static void OnImGuiRender() {
			instance->m_ScenePanel.OnImGuiRender();
			//instance->m_ActionPanel.OnImGuiRender();
			instance->m_AssetsPanel.OnImGuiRender();
			instance->m_PropertiesPanel.OnImGuiRender();
			instance->m_MaterialPanel.OnImGuiRender();
			instance->m_BluePrintEditor.OnImGuiRender();
		}
		static void Init();
		static ScenePanel* GetScenePanel() { return &instance->m_ScenePanel; }
		static ActionPanel* GetActionPanel() { return &instance->m_ActionPanel; }
		static AssetsPanel* GetAssetsPanel() { return &instance->m_AssetsPanel; }
		static PropertiesPanel* GetPropertiesPanel() { return &instance->m_PropertiesPanel; }
		static MaterialPanel* GetMaterialPanel() { return &instance->m_MaterialPanel; }
		static BluePrintEditor* GetBluePrintEditor() { return &instance->m_BluePrintEditor; }
	
	};
	

}