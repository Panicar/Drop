#include "PlatformWindows.hpp"
#include "Log/Log.hpp"


namespace drop
{
	void PlatformWindows::InitializeImpl(void* instance)
	{
		DP_CORE_INFO("Platform initialized!");

		// Register window class
		WNDCLASS wndClass = {};
		wndClass.style = CS_DBLCLKS;

	}

	void PlatformWindows::TerminateImpl()
	{
		DP_CORE_INFO("Platform terminated!");
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (hWnd != nullptr)
		{
			
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}