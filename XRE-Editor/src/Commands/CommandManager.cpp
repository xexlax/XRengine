
#include "CommandManager.h"

namespace XRE {
	CommandManager  CommandManager::instance;
	

	void CommandManager::Command_CreateObj(GameObject go)
	{
		XRef<EditorCommandCreateGameObj> command= XMakeRef<EditorCommandCreateGameObj>(go);
		UndoStack.push_back(command);
		RedoStack.clear();
	}

	void CommandManager::Command_DeleteObj(GameObject go)
	{
		XRef<EditorCommandDeleteGameObj> command = XMakeRef<EditorCommandDeleteGameObj>(go);
		UndoStack.push_back(command);
		RedoStack.clear();
	}

	void CommandManager::Undo()
	{
		if (!UndoStack.empty()) {
			XRef<EditorCommand> command = UndoStack.back();
			command->UnExecute();
			RedoStack.push_back(command);
			UndoStack.pop_back();
			
			if (RedoStack.size() > MaxStackSize) RedoStack.pop_front();
		}
	}
	void CommandManager::Redo()
	{
		if (!RedoStack.empty()) {
			XRef<EditorCommand> command = RedoStack.back();
			command->Execute();
			UndoStack.push_back(command);
			RedoStack.pop_back();
			
			if (UndoStack.size() > MaxStackSize) UndoStack.pop_front();
		}
	}
}