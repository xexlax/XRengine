#pragma once
#include "EditorCommand.h"

#include <XRE.h>
#include <deque>

namespace XRE {
	class CommandManager {
		friend class ActionPanel;
		
	public:
		static CommandManager instance;
		std::unordered_map<EditorCommand*, void*> Handles;
		std::unordered_map<EditorCommand*, GameObject> Gos;
		
		void AddHandle(EditorCommand* command, void* handle);
		void UpdateHandle(void* origin, void* replaced);

		void AddGO(EditorCommand* command, GameObject handle);
		void UpdateGO(GameObject origin, GameObject replaced);
		void DelayDestroyGO(GameObject go);


		void OnUpdate();
		void OnCommandAdd(XRef<EditorCommand> command);



		inline static CommandManager& Get() { return instance; }
		//创建/删除指令
		void Command_CreateObj(GameObject go);

		void Command_DeleteObj(GameObject go);

		template <typename T>
		void Command_Create_Component(T& c, GameObject go);

		template <typename T>
		void Command_Delete_Component(T& c, GameObject go);


		

		//产生编辑值指令
	

		//单次编辑指令
		template <typename T>
		void Command_Edit(T* handle, T value_old, T value_new);
		
		//连续型编辑指令（Drag或者Gizmos操作）
		template <typename T>
		void Begin_Command_Edit(T* handle, T value_old);
		template <typename T>
		void End_Command_Edit(T* handle,  T value_new);


		//Todo 粘贴组件值（反序列化）指令、 指令组

		void Clear();

		void Undo();
		void Redo();

		bool IsMakingCommand() const { return Pending != nullptr; }
		bool CanUndo() const { return !UndoStack.empty(); }
		bool CanRedo() const { return !RedoStack.empty(); }


	protected:

		uint32_t MaxStackSize = 30;

		XRef<EditorCommand> Pending = nullptr;

		//UndoStack 新指令在后，RedoStack 新指令在前，方便遍历
		std::deque<XRef<EditorCommand>> UndoStack;
		std::deque<XRef<EditorCommand>> RedoStack;
		std::vector<GameObject> PendingToDestroy;


		
		
	};
	template<typename T>
	inline void CommandManager::Command_Create_Component(T& c, GameObject go)
	{
		auto command = XMakeRef<EditorCommandCreateComponent<T>>(c, go);
		OnCommandAdd(command);
	}
	template<typename T>
	inline void CommandManager::Command_Delete_Component(T& c, GameObject go)
	{
		auto command = XMakeRef<EditorCommandDeleteComponent<T>>(c, go);
		OnCommandAdd(command);
	}
	template<typename T>
	inline void CommandManager::Command_Edit(T* handle, T value_old, T value_new)
	{
		auto command = XMakeRef<EditorCommandEdit<T>>(handle, value_old, value_new);
		command->Execute();
		OnCommandAdd(command);
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
				OnCommandAdd(command);
				Pending = nullptr;
			}
			
		}
		
		
	}
}