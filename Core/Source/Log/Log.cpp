#include "Log.hpp"

#include <filesystem>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <iostream>

namespace DP {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger = nullptr;
	std::shared_ptr<spdlog::logger> Log::s_AppLogger = nullptr;

	void Log::Initialize()
	{
		try
		{
			std::filesystem::create_directories("Logs");

			std::vector<spdlog::sink_ptr> coreSinks;
			coreSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			coreSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Drop.log", true));

			coreSinks[0]->set_pattern("%^[%T] %n: %v%$");
			coreSinks[1]->set_pattern("[%T] [%l] %n: %v");

			s_CoreLogger = std::make_shared<spdlog::logger>("CORE", begin(coreSinks), end(coreSinks));
			s_CoreLogger->set_level(spdlog::level::trace);
			s_CoreLogger->flush_on(spdlog::level::trace);

			std::vector<spdlog::sink_ptr> appSinks;
			appSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			appSinks.emplace_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("Logs/App.log", 10 * 1024 * 1024, 5));

			appSinks[0]->set_pattern("%^[%T] %n: %v%$");
			appSinks[1]->set_pattern("[%T] [%l] %n: %v");

			s_AppLogger = std::make_shared<spdlog::logger>("App", begin(appSinks), end(appSinks));
			s_AppLogger->set_level(spdlog::level::trace);
			s_AppLogger->flush_on(spdlog::level::trace);

			spdlog::register_logger(s_CoreLogger);
			spdlog::register_logger(s_AppLogger);
		}
		catch (const spdlog::spdlog_ex& ex) 
		{
			std::cerr << "Log initialization failed: " << ex.what() << '\n';
		}

	}

	void Log::Shutdown()
	{
		spdlog::shutdown();
	}


}