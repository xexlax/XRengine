#pragma once
#include <initializer_list>
#include <functional>
#include <sstream>
#include <XRE.h>
namespace XRE {
	
	class EditorCommand {

	public:
		
		virtual void Execute()=0;
		virtual void UnExecute()=0;
	};

	template <typename T>
	class EditorCommandCreateComponent:public EditorCommand {

		XRef<T> m_Handle;
		GameObject m_go;
		std::stringstream sstream;


	public:

		EditorCommandCreateComponent(T& c, GameObject go) {
			cereal::JSONOutputArchive oarchive(sstream);
			oarchive(c);


			m_Handle = XMakeRef<T>(c);
			m_go = go;
		}
		
		virtual void Execute() override {
			
			sstream.seekg(0, ios::beg);
			cereal::JSONInputArchive iarchive(sstream);
			
			m_Handle = XMakeRef<T>(m_go.AddComponent<T>());
			iarchive(*m_Handle);
			
		}
		virtual void UnExecute() override {
			m_go.RemoveComponent<T>();
			m_Handle = nullptr;
		}
	};

	template <typename T>
	class EditorCommandDeleteComponent : public EditorCommand {
		XRef<T> m_Handle;
		GameObject m_go;
		std::stringstream sstream;
	public:
		EditorCommandDeleteComponent(T& c ,GameObject go) {
			
			cereal::JSONOutputArchive oarchive(sstream);
			oarchive(c);
			m_Handle = XMakeRef<T>(c);
			m_go = go;
		}
		virtual void Execute() override {
			m_go.RemoveComponent<T>();
			m_Handle = nullptr;
		}
		virtual void UnExecute() override {
			sstream.seekg(0, ios::beg);
			cereal::JSONInputArchive iarchive(sstream);
			m_Handle = XMakeRef<T>(m_go.AddComponent<T>());
			iarchive(*m_Handle);
		}
	};


	
	class EditorCommandCreateGameObj :public EditorCommand {


		std::stringstream sstream;
		GameObject m_go;
		Scene* m_sc;


	public:
		EditorCommandCreateGameObj(GameObject g);
		virtual void Execute() override;
		virtual void UnExecute() override;
	};


	class EditorCommandDeleteGameObj : public EditorCommand {
		

		std::stringstream sstream;
		GameObject m_go;
		Scene* m_sc;
	public:
		EditorCommandDeleteGameObj(GameObject g);
		virtual void Execute() override;
		virtual void UnExecute() override;
	};


	





	template <typename T>
	class EditorCommandEdit : public EditorCommand {
	public:

		T* m_Handle;
		T value_old, value_new;

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
		virtual void Execute() override;
		virtual void UnExecute() override;

	};
}