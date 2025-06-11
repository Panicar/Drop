#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace DP {

	class Log
	{
	public:

		static void Initialize();
		static void Shutdown();
		
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_AppLogger; }

	private:
		
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;

	};

}

// Core (Framework) logging macros
#define DP_CORE_TRACE(...)			DP::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DP_CORE_INFO(...)			DP::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DP_CORE_WARN(...)			DP::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DP_CORE_ERROR(...)			DP::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DP_CORE_CRITICAL(...)		DP::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Application logging macros
#define DP_APP_TRACE(...)			DP::Log::GetAppLogger()->trace(__VA_ARGS__)
#define DP_APP_INFO(...)			DP::Log::GetAppLogger()->info(__VA_ARGS__)
#define DP_APP_WARN(...)			DP::Log::GetAppLogger()->warn(__VA_ARGS__)
#define DP_APP_ERROR(...)			DP::Log::GetAppLogger()->error(__VA_ARGS__)
#define DP_APP_CRITICAL(...)		DP::Log::GetAppLogger()->critical(__VA_ARGS__)