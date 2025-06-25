#include "Application.hpp"
#include "Log/Log.hpp"
#include <iostream>

namespace drop {


	CoreApplication* CoreApplication::s_Instance = nullptr;

	CoreApplication::CoreApplication()
	{
		if (s_Instance)
		{
			DP_CORE_ERROR("Application has already exists!");
			//__debugbreak();
		}

		s_Instance = this;

		DP_CORE_INFO("Application has created!");

	}

	CoreApplication::~CoreApplication()
	{

	}

	void CoreApplication::Execute()
	{
		char a = {};

		while (m_IsRunning)
		{
			OnUpdate();
			
			std::cin >> a;
			if (a == 'q')
			{
				m_IsRunning = false;
				break;
			}
			
		}
	}
		
	void CoreApplication::Print()
	{
		std::cout << "Application Print ... " << '\n';

	}
}
