#include "Platform.hpp"
#include "Log/Log.hpp"

#include "Core.hpp"
#ifdef DP_PLATFORM_WINDOWS
	#include "Windows/PlatformWindows.hpp"
	using Platform = drop::PlatformWindows;
#elif DP_PLATFORM_LINUX
	#include "Linux/PlatformLinux.hpp"
	uinsg Platform = drop::PlatformLinux;
#endif


	namespace drop {


	std::unique_ptr<IPlatform> IPlatform::s_Instance = nullptr;

	void IPlatform::Initialize(void* instance)
	{
		if (s_Instance)
		{
			DP_CORE_ERROR("Platform instance has already exists!");
			assert(1);
		}

		s_Instance = IPlatform::Create();

		s_Instance->InitializeImpl(instance);
	}

	std::unique_ptr<IPlatform> IPlatform::Create()
	{
		return std::make_unique<Platform>();
	}

}