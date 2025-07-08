#pragma once

#include "Core.hpp"
#include "Platform/Base/Window.hpp"

#include <string>

namespace drop 
{

	struct CommandLineArgs
	{
		// TODO: fix here
		int Argc = 0;
		char** Argv = nullptr;
	};

	class CoreApp
	{
	public:

		CoreApp(const CommandLineArgs& args);
		virtual ~CoreApp();

		CoreApp(const CoreApp& other) = delete;
		CoreApp(CoreApp&& other) = delete;
		CoreApp& operator=(const CoreApp& other) = delete;
		CoreApp& operator=(CoreApp&& other) = delete;

		void Update() {}
		
		void Run();

		// Testing
		void Print();
		
		static CoreApp* Instance() { return s_Instance; }

	private:
		static CoreApp* s_Instance;
		bool m_IsRunning = true;

		IWindow* m_Window = nullptr;
		
	};

	CoreApp* CreateApp(const CommandLineArgs& args);

}