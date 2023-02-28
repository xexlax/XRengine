#pragma once
#include <string>
#include <vector>

namespace XRE {
	//±à¼­Æ÷ÄÚÖÃµÄlog system
	enum MsgType {
		XRE_CONSOLE_LOG=0,
		XRE_CONSOLE_WARN,
		XRE_CONSOLE_ERROR
	};
	struct  ConsoleMsg
	{
		MsgType type;
		std::string content;
		ConsoleMsg(MsgType t, std::string c) {
			type = t;
			content = c;
		}

	};
	class Console {

	public:
		static void Log(std::string s);
		static void Warn(std::string s);
		static void Error(std::string s);

		static void Clear();


	private:
		static std::vector<ConsoleMsg> m_MsgBuffer;

	};

}
