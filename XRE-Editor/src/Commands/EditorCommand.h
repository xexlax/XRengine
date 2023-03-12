#pragma once
#include <initializer_list>
#include <functional>
#include <sstream>
#include <XRE.h>
namespace XRE {
	
	class EditorCommand {
	protected:
		std::string m_String;
		
	public:
		virtual ~EditorCommand();
		void* GetHandle();
		GameObject GetGO();
		virtual void Execute()=0;
		virtual void UnExecute()=0;
		virtual std::string GetString()= 0;
	};

	template <typename T>
	class EditorCommandCreateComponent:public EditorCommand {
		
		std::stringstream sstream;

	public:
		virtual std::string GetString()override {
			return u8"为对象 " + GetGO().GetName() + u8" 添加组件：" + m_String;
		}


		EditorCommandCreateComponent(T& c, GameObject go) {

			cereal::JSONOutputArchive oarchive(sstream);
			oarchive(c);
			//CommandManager::Get().AddHandle(this, &c);
			CommandManager::Get().AddGO(this, go);
			m_String = c.m_Name;
		
		}
		
		virtual void Execute() override {
			
			sstream.seekg(0, ios::beg);
			cereal::JSONInputArchive iarchive(sstream);

			T& c=GetGO().AddComponent<T>();
			iarchive(c);
			
		}
		virtual void UnExecute() override {
			GetGO().RemoveComponent<T>();
		}
	};

	template <typename T>
	class EditorCommandDeleteComponent : public EditorCommand {
		std::string m_String;
		std::stringstream sstream;
	public:
		virtual std::string GetString()override {
			return  u8"为对象 " + GetGO().GetName() + u8" 删除组件：" + m_String;
		}

		EditorCommandDeleteComponent(T& c ,GameObject go) {
			
			
			cereal::JSONOutputArchive oarchive(sstream);
			oarchive(c);
			CommandManager::Get().AddGO(this, go);
			m_String = c.m_Name;

		}
		virtual void Execute() override {
			GetGO().RemoveComponent<T>();
		}
		virtual void UnExecute() override {
			sstream.seekg(0, ios::beg);
			cereal::JSONInputArchive iarchive(sstream);
			T& c = GetGO().AddComponent<T>();
			iarchive(c);
		}
	};


	
	class EditorCommandCreateGameObj :public EditorCommand {


		std::stringstream sstream;
		Scene* m_sc;


	public:

		virtual std::string GetString() override {
			return  u8"新建对象 " + m_String ;
		}
		EditorCommandCreateGameObj(GameObject g);
		virtual void Execute() override;
		virtual void UnExecute() override;
	};


	class EditorCommandDeleteGameObj : public EditorCommand {
		

		std::stringstream sstream;
		Scene* m_sc;
	public:
		virtual std::string GetString() override {
			return  u8"删除对象 " + m_String ;
		}
		EditorCommandDeleteGameObj(GameObject g);
		virtual void Execute() override;
		virtual void UnExecute() override;
	};


	





	template <typename T>
	class EditorCommandEdit : public EditorCommand {
	public:

		T* m_Handle;
		T value_old, value_new;

		virtual std::string GetString() override {
			if (!m_Handle) return u8"已失效的编辑区域" + std::string(typeid(T).name());
			return  u8"编辑 " + std::string(typeid(T).name());
		}

		EditorCommandEdit(T* h,T o, T n)
		{
			m_Handle = h;
			value_old = o;
			value_new = n;
		}

		virtual void Execute() override
		{
			if (m_Handle)
				*m_Handle = value_new;
		}
		virtual void UnExecute() override
		{
			if (m_Handle)
				*m_Handle = value_old;
		}
	};

	class MultiEditorCommand :public EditorCommand {

		std::deque<XRef<EditorCommand>> m_List;
	public:
		virtual std::string GetString() override {
			return  u8"复合指令";
		}

		virtual void Execute() override;
		virtual void UnExecute() override;

	};
}