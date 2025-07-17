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

	class CoreApplication
	{
	public:

		CoreApplication(const CommandLineArgs& args);
		virtual ~CoreApplication();

		CoreApplication(const CoreApplication& other) = delete;
		CoreApplication(CoreApplication&& other) = delete;
		CoreApplication& operator=(const CoreApplication& other) = delete;
		CoreApplication& operator=(CoreApplication&& other) = delete;

		void OnResize(int width, int height);
		bool OnKeyCallback(Key key, char charCode, Action action, bool isRepeat, Modifier mods);
		
		void Update() {}
		void Run();
		// Testing
		void Print();
		
		static CoreApplication* Instance() { return s_Instance; }

	private:
		static CoreApplication* s_Instance;
		bool m_IsRunning = true;

		IWindow* m_Window = nullptr;
		IWindow* m_Window2 = nullptr;

	};

	CoreApplication* CreateApplication(const CommandLineArgs& args);

}