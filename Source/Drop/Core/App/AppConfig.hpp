#pragma once

#include "Core.hpp"
#include "Log/Log.hpp"

namespace drop
{
	class IAppConfig
	{
	public:

		virtual ~IAppConfig()
		{
			DP_CORE_ERROR("Application CONFIG DELETED!!!");
		}
	};

	class ConsoleAppConfig : public IAppConfig
	{
	public:
		int argc = 0;
		char** argv = nullptr;
	};

	#ifdef DP_PLATFORM_WINDOWS

	class WinAppConfig : public IAppConfig
	{
	public:
		HINSTANCE hInstance = nullptr;
		HINSTANCE hPrevInstance = nullptr;
		LPSTR lpCmdLine = nullptr;
		int nCmdShow = 0;
	};
	
	#endif

}