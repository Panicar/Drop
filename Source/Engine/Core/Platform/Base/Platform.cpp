#include "Platform.hpp"
#include "Core.hpp"


#ifdef DP_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsPlatform.hpp"
	using Platform = drop::WindowsPlatform;
#elif DP_PLATFORM_LINUX
	#include "Platform/Linux/PlatformLinux.hpp"
	uinsg Platform = drop::PlatformLinux;
#endif


	namespace drop 
	{


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