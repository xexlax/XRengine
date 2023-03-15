
#include "CommandManager.h"
#include "../EditorPanels/PanelsManager.h"
namespace XRE {
	CommandManager  CommandManager::instance;
	

	void CommandManager::AddHandle(EditorCommand* command, void* handle)
	{
		Handles[command] = handle;
	}

	void CommandManager::UpdateHandle(void* origin, void* replaced)
	{
		for (auto x : Handles) {
			if (x.second == origin) x.second = replaced;
		}
	}

	void CommandManager::AddGO(EditorCommand* command, GameObject handle)
	{
		Gos[command] = handle;
	}

	void CommandManager::UpdateGO(GameObject origin, GameObject replaced)
	{
		for (auto& x : Gos) {
			if (x.second == origin) 
				x.second = replaced;
		}
	}

	void CommandManager::DelayDestroyGO(GameObject go)
	{
		PendingToDestroy.push_back(go);
	}

	void CommandManager::OnUpdate()
	{
		for (auto go : PendingToDestroy) {
			if (PanelsManager::GetScenePanel()->GetSelected() == go) {
				PanelsManager::GetScenePanel()->UnSelect();
			}
			go.GetScene()->Destroy(go);
		}
		PendingToDestroy.clear();
	}

	void CommandManager::OnCommandAdd(XRef<EditorCommand> command)
	{
		PanelsManager::GetActionPanel()->m_updated = true;
		UndoStack.push_back(command);
		if (UndoStack.size() > MaxStackSize) UndoStack.pop_front();
		RedoStack.clear();
	}

	void CommandManager::Command_CreateObj(GameObject go)
	{
		XRef<EditorCommandCreateGameObj> command= XMakeRef<EditorCommandCreateGameObj>(go);
		OnCommandAdd(command);
		
	}

	void CommandManager::Command_DeleteObj(GameObject go)
	{
		XRef<EditorCommandDeleteGameObj> command = XMakeRef<EditorCommandDeleteGameObj>(go);
		OnCommandAdd(command);
	}

	void CommandManager::Clear()
	{
		UndoStack.clear();
		RedoStack.clear();
		Pending = nullptr;
	}

	void CommandManager::Undo()
	{
		if (!UndoStack.empty()) {
			XRef<EditorCommand> command = UndoStack.back();
			command->UnExecute();
			RedoStack.push_front(command);
			UndoStack.pop_back();
			
			if (RedoStack.size() > MaxStackSize) RedoStack.pop_back();
		}
	}
	void CommandManager::Redo()
	{
		if (!RedoStack.empty()) {
			XRef<EditorCommand> command = RedoStack.front();
			command->Execute();
			UndoStack.push_back(command);
			RedoStack.pop_front();
			
			if (UndoStack.size() > MaxStackSize) UndoStack.pop_front();
		}
	}
}