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

		WindowProperties props = {};
		props.Title = "Drop Engine";
		props.DefaultTitlebar = false;
		//props.IsBorderless = true;
		m_Window = IWindow::Create(props);

	}

	CoreApp::~CoreApp()
	{
		delete m_Window;
	}

	void CoreApp::Run()
	{
		while (m_IsRunning)
		{
			Update();
		
			m_Window->Update();
		}
	}
}
