#pragma once
#include "pch.h"
#include "Macros.h"
#include "spdlog\spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
//macros
#define XRE_CORE_WARN(...) ::XRE::Log::getCoreLogger()->warn(__VA_ARGS__)
#define XRE_CORE_ERROR(...) ::XRE::Log::getCoreLogger()->error(__VA_ARGS__)
#define XRE_CORE_INFO(...) ::XRE::Log::getCoreLogger()->info(__VA_ARGS__)
#define XRE_CORE_TRACE(...) ::XRE::Log::getCoreLogger()->trace(__VA_ARGS__)
#define XRE_CORE_FATAL(...) ::XRE::Log::getCoreLogger()->fatal(__VA_ARGS__)

#define XRE_WARN(...) ::XRE::Log::getClientLogger()->warn(__VA_ARGS__)
#define XRE_ERROR(...) ::XRE::Log::getClientLogger()->error(__VA_ARGS__)
#define XRE_INFO(...) ::XRE::Log::getClientLogger()->info(__VA_ARGS__)
#define XRE_TRACE(...) ::XRE::Log::getClientLogger()->trace(__VA_ARGS__)
#define XRE_FATAL(...) ::XRE::Log::getClientLogger()->fatal(__VA_ARGS__)

namespace XRE {
	class Log
	{
	public:
		
		static void Init();
		inline static Ref<spdlog::logger> getCoreLogger() { return s_coreLogger; }
		inline static Ref<spdlog::logger> getClientLogger() { return s_clientLogger; }

	private:
		static Ref<spdlog::logger> s_coreLogger;
		static Ref<spdlog::logger> s_clientLogger;
	};

}

