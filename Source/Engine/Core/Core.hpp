#pragma once


#ifdef _WIN32

	#ifdef _WIN64
		#define DP_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported."
	#endif

#endif

#ifdef __linux__
#define DP_PLATFORM_LINUX
#endif

#ifdef DP_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX   
#include <windows.h>
#undef ZeroMemory
#undef CopyMemory
#endif