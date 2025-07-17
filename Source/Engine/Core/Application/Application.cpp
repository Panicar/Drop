#include "Application.hpp"
#include "Log/Log.hpp"
#include "Platform/Base/Platform.hpp"
#include <iostream>

namespace drop
{
	CoreApplication* CoreApplication::s_Instance = nullptr;

	CoreApplication::CoreApplication(const CommandLineArgs& args)
	{
		if (s_Instance)
		{
			DP_CORE_ERROR("Application has already exists!");
			//__debugbreak();
		}

		s_Instance = this;

		DP_CORE_INFO("Application has created!");

		WindowProperties props = {};
		props.Title = "Drop Engine";
		props.DefaultTitlebar = true;

		props.Callbacks.Close = [&]() {
			DP_CORE_INFO("Window close callback triggered!");
			m_IsRunning = false;
		};

		//props.IsBorderless = true;
		m_Window = IWindow::Create(props);

		props.DefaultTitlebar = true;
		props.Title = "Drop Engine - Second Window";
		//m_Window2 = IWindow::Create(props);

		m_Window->SetWindowResizeCallback(std::bind(&CoreApplication::OnResize, this, std::placeholders::_1, std::placeholders::_2));
		m_Window->SetKeyCallback(std::bind(&CoreApplication::OnKeyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

	}

	CoreApplication::~CoreApplication()
	{
		delete m_Window;
	}

	void CoreApplication::OnResize(int width, int height)
	{
		DP_CORE_INFO("Window Resized: ({}, {})", width, height);
	}

	bool CoreApplication::OnKeyCallback(Key key, char charCode, Action action, bool isRepeat, Modifier mods)
	{
		//DP_CORE_INFO("Key: {}, CharCode: {}, Action: {}, IsRepeat: {}, ScanCode: {}", static_cast<int>(key), charCode, static_cast<int>(action), isRepeat, (char)scanCode);

		if (action == Action::Press)
		{
			DP_CORE_INFO("Key Pressed: {}", static_cast<int>(key));

			if (key == Key::Enter)
			{
				DP_CORE_ERROR("Enter key pressed!");
			}

			if(key == Key::W && (mods & Modifier::LeftControl))
			{
				DP_CORE_ERROR("Ctrl + W pressed!");
				if (m_Window2)
				{
					delete m_Window2;
					m_Window2 = nullptr;
				}
			}

			if ((key == Key::R) &&
				(mods & Modifier::LeftControl) &&
				(mods & Modifier::LeftShift))
			{
				DP_CORE_ERROR("Ctrl + Shift + R pressed!");
			}

		}

		if( action == Action::Release)
		{
			DP_CORE_INFO("Key Released: {}", static_cast<int>(key));
		}


		return true;
	}

	void CoreApplication::Run()
	{
		while (m_IsRunning)
		{
			IPlatform::PumpMessages();
	
			Update();

			/*if (GetAsyncKeyState('Q') & 0x8000)
			{
				DP_CORE_DEBUG("'Q' is being pressed");
				break;
			}*/

			//m_Window2->SetMouseCursor(CursorType::SizeWE);

		}
	}
}
