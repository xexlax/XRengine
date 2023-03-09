#pragma once
#include "EditorCommand.h"
#include <XRE.h>
#include <deque>

namespace XRE {
	class CommandManager {
	public:
		static CommandManager instance;

		


		inline static CommandManager& Get() { return instance; }

		void Command_CreateObj(GameObject go);
		void Command_DeleteObj(GameObject go);

		template <typename T>
		void Command_Create_Component(T& c, GameObject go);

		template <typename T>
		void Command_Delete_Component(T& c, GameObject go);


		template <typename T>
		void Command_Edit(T* handle, T value_old, T value_new);
		
		template <typename T>
		void Begin_Command_Edit(T* handle, T value_old);

		template <typename T>
		void End_Command_Edit(T* handle,  T value_new);



		void Undo();
		void Redo();

		bool IsMakingCommand() const { return Pending != nullptr; }
		bool CanUndo() const { return !UndoStack.empty(); }
		bool CanRedo() const { return !RedoStack.empty(); }

	protected:

		uint32_t MaxStackSize = 30;

		XRef<EditorCommand> Pending = nullptr;
		std::deque<XRef<EditorCommand>> UndoStack;
		std::deque<XRef<EditorCommand>> RedoStack;
	};
	template<typename T>
	inline void CommandManager::Command_Create_Component(T& c, GameObject go)
	{
		auto command = XMakeRef<EditorCommandCreateComponent<T>>(c, go);
		UndoStack.push_back(command);
		RedoStack.clear();
	}
	template<typename T>
	inline void CommandManager::Command_Delete_Component(T& c, GameObject go)
	{
		auto command = XMakeRef<EditorCommandDeleteComponent<T>>(c, go);
		UndoStack.push_back(command);
		RedoStack.clear();
	}
	template<typename T>
	inline void CommandManager::Command_Edit(T* handle, T value_old, T value_new)
	{
		auto command = XMakeRef<EditorCommandEdit<T>>(handle, value_old, value_new);
		command->Execute();
		UndoStack.push_back(command);
		if (UndoStack.size() > MaxStackSize) UndoStack.pop_front();

		RedoStack.clear();
	}
	template<typename T>
	inline void CommandManager::Begin_Command_Edit(T* handle, T value_old)
	{
		auto command = XMakeRef<EditorCommandEdit<T>>(handle, value_old, value_old);
		Pending = command;
		
	}
	template<typename T>
	inline void CommandManager::End_Command_Edit(T* handle, T value_new)
	{
		if (Pending ) {
			XRef<EditorCommandEdit<T>> command = std::dynamic_pointer_cast<EditorCommandEdit<T>>(Pending);
			if (command) {
				if (handle == command->m_Handle)
					command->value_new = value_new;
				UndoStack.push_back(command);
				RedoStack.clear();
				Pending = nullptr;
			}
			
		}
		
		
	}
}