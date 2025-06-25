#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace drop {

	class Log
	{
	public:

		Log() = delete;
		~Log() = delete;
		Log(const Log& other) = delete;
		Log(Log&& other) = delete;
		Log& operator=(Log && other) = delete;
		Log& operator=(const Log& other) = delete;

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
#define DP_CORE_TRACE(...)			drop::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DP_CORE_INFO(...)			drop::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DP_CORE_WARN(...)			drop::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DP_CORE_ERROR(...)			drop::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DP_CORE_CRITICAL(...)		drop::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Application logging macros
#define DP_APP_TRACE(...)			drop::Log::GetAppLogger()->trace(__VA_ARGS__)
#define DP_APP_INFO(...)			drop::Log::GetAppLogger()->info(__VA_ARGS__)
#define DP_APP_WARN(...)			drop::Log::GetAppLogger()->warn(__VA_ARGS__)
#define DP_APP_ERROR(...)			drop::Log::GetAppLogger()->error(__VA_ARGS__)
#define DP_APP_CRITICAL(...)		drop::Log::GetAppLogger()->critical(__VA_ARGS__)