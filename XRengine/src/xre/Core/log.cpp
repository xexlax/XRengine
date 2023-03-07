#include "pch.h"


namespace XRE {
	XRef<spdlog::logger> Log::s_coreLogger;
	XRef<spdlog::logger> Log::s_clientLogger;
	void Log::Init()
	{	
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("XRE");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);

	
	}
	
}