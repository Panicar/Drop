#include "App.hpp"
#include "Log/Log.hpp"
#include <iostream>

namespace drop
{


	CoreApp* CoreApp::s_Instance = nullptr;

	CoreApp::CoreApp(const CommandLineArgs& args)
	{
		if (s_Instance)
		{
			DP_CORE_ERROR("App has already exists!");
			//__debugbreak();
		}

		s_Instance = this;

		DP_CORE_INFO("App has created!");

		WindowProperties props;
		m_Window = IWindow::Create(props);

	}

	CoreApp::~CoreApp()
	{
		delete m_Window;
	}

	void CoreApp::Run()
	{
		char a = {};

		while (m_IsRunning)
		{
			Update();
		
			m_Window->Update();
			
		}
	}
		
	void CoreApp::Print()
	{
		DP_CORE_DEBUG("[APP]: CoreApp::Print()");
	}
}
