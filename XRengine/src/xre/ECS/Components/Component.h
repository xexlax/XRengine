#pragma once

namespace XRE {

	class Component
	{
	public:
		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();

	protected:
		std::string m_Name;
		bool m_Active;

	};

}

