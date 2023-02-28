#include "pch.h"
#include "Console.h"

namespace XRE {
	std::vector<ConsoleMsg> Console::m_MsgBuffer;

	void Console::Log(std::string s)
	{
		ConsoleMsg m(XRE_CONSOLE_LOG, s);
		m_MsgBuffer.push_back(m);
	}
	void Console::Warn(std::string s)
	{
		ConsoleMsg m(XRE_CONSOLE_LOG, s);
		m_MsgBuffer.push_back(m);
	}
	void Console::Error(std::string s)
	{
		ConsoleMsg m(XRE_CONSOLE_LOG, s);
		m_MsgBuffer.push_back(m);
	}
	void Console::Clear()
	{
		m_MsgBuffer.clear();
	}
}