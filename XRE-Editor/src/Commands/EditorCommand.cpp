#include "EditorCommand.h"
#include "cereal\archives\json.hpp"

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
		
		cereal::JSONOutputArchive oarchive(sstream);
		oarchive(g);
		m_go = g;
		m_sc = g.GetScene();
	}

	void EditorCommandCreateGameObj::Execute()
	{
		m_go = m_sc->CreateGameObject("0");
		
		sstream.seekg(0, ios::beg);
		cereal::JSONInputArchive iarchive(sstream);
		iarchive(m_go);
		
	}

	void EditorCommandCreateGameObj::UnExecute()
	{
		if(m_go)
		m_sc->Destroy(m_go);
		m_go = GameObject();
	}

	EditorCommandDeleteGameObj::EditorCommandDeleteGameObj(GameObject g)
	{
		
		cereal::JSONOutputArchive oarchive(sstream);
		oarchive(g);
	
		m_sc = g.GetScene();
	}

	void EditorCommandDeleteGameObj::Execute()
	{
		if(m_go)
		m_sc->Destroy(m_go);
		m_go = GameObject();
	}

	void EditorCommandDeleteGameObj::UnExecute()
	{
		m_go = m_sc->CreateGameObject("0");
		

		sstream.seekg(0, ios::beg);
		
		cereal::JSONInputArchive iarchive(sstream);
		
		iarchive(m_go);
	}

	

}