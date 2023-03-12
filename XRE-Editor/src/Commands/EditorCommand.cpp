#include "EditorCommand.h"
#include "cereal\archives\json.hpp"
#include "CommandManager.h"

namespace XRE {
	




	void MultiEditorCommand::Execute()
	{
		for (auto x = m_List.begin();x != m_List.end();x++) {
			(*x)->Execute();
		}
	}

	void MultiEditorCommand::UnExecute()
	{
		for (auto x = m_List.end();x != m_List.begin();) {
			(*(--x))->UnExecute();
		}
	}

	EditorCommandCreateGameObj::EditorCommandCreateGameObj(GameObject g)
	{
		m_String = g.GetName();
		cereal::JSONOutputArchive oarchive(sstream);
		oarchive(g);
		CommandManager::Get().AddGO(this, g);
		m_sc = g.GetScene();
	}

	void EditorCommandCreateGameObj::Execute()
	{
		GameObject m_go = m_sc->CreateGameObject("0");
		sstream.seekg(0, ios::beg);
		cereal::JSONInputArchive iarchive(sstream);
		iarchive(m_go);

		CommandManager::Get().UpdateGO(GetGO(), m_go);
		
	}

	void EditorCommandCreateGameObj::UnExecute()
	{
		if(GetGO())
			CommandManager::Get().DelayDestroyGO(GetGO());
	}

	EditorCommandDeleteGameObj::EditorCommandDeleteGameObj(GameObject g)
	{
		m_String = g.GetName();
		cereal::JSONOutputArchive oarchive(sstream);
		oarchive(g);
		CommandManager::Get().AddGO(this, g);
		m_sc = g.GetScene();
	}

	void EditorCommandDeleteGameObj::Execute()
	{
		if (GetGO())
			CommandManager::Get().DelayDestroyGO(GetGO());
	}

	void EditorCommandDeleteGameObj::UnExecute()
	{
		GameObject m_go = m_sc->CreateGameObject("0");
		

		sstream.seekg(0, ios::beg);
		
		cereal::JSONInputArchive iarchive(sstream);
		
		iarchive(m_go);

		CommandManager::Get().UpdateGO(GetGO(), m_go);
	}

	

	EditorCommand::~EditorCommand()
	{
		CommandManager::Get().Handles.erase(this);
	}

	void* EditorCommand::GetHandle()
	{
		return CommandManager::Get().Handles[this];
	}

	GameObject EditorCommand::GetGO()
	{
		return CommandManager::Get().Gos[this];
	}

}