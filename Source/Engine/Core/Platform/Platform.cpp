#include "Platform.hpp"
#include "Core.hpp"


#ifdef DP_PLATFORM_WINDOWS
	#include "Windows/PlatformWindows.hpp"
	using Platform = drop::PlatformWindows;
#elif DP_PLATFORM_LINUX
	#include "Linux/PlatformLinux.hpp"
	uinsg Platform = drop::PlatformLinux;
#endif


	namespace drop {


	IPlatform* IPlatform::s_Instance = nullptr;

	void IPlatform::Initialize()
	{
		s_Instance->InitializeImpl();
	}

	IPlatform* IPlatform::Create(void* hInstance)
	{
		if (s_Instance)
		{
			DP_CORE_ERROR("Platform instance has already exists!");
			assert(1);
		}

		s_Instance = new Platform(hInstance);

		return s_Instance;
	}

}