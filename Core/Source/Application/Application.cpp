#include "Application.hpp"

#include <iostream>

#include <spdlog/spdlog.h>

namespace DP {


	CoreApplication* CoreApplication::s_Instance = nullptr;

	CoreApplication::CoreApplication()
	{
		if (s_Instance)
		{
			spdlog::error("Application has already exists!");
			__debugbreak();
		}

		s_Instance = this;

		spdlog::info("Application has created!");
	}

	CoreApplication::~CoreApplication()
	{

	}

	void CoreApplication::Execute()
	{
		while (true)// <- Is Application Running 
		{
			OnUpdate();
		}
	}

	void CoreApplication::Print()
	{
		std::cout << "Application Print ... " << '\n';

	}
}