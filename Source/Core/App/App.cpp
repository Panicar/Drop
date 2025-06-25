#include "App.hpp"
#include "Log/Log.hpp"
#include <iostream>

namespace drop {


	CoreApp* CoreApp::s_Instance = nullptr;

	CoreApp::CoreApp()
	{
		if (s_Instance)
		{
			DP_CORE_ERROR("App has already exists!");
			//__debugbreak();
		}

		s_Instance = this;

		DP_CORE_INFO("App has created!");

	}

	CoreApp::~CoreApp()
	{

	}

	void CoreApp::Execute()
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
		
	void CoreApp::Print()
	{
		std::cout << "App Print ... " << '\n';

	}
}
