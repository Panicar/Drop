#include "PlatformWindows.hpp"
#include "Log/Log.hpp"

namespace drop
{
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	PlatformWindows::PlatformWindows(void* hInstance)
		: m_HInstance((HINSTANCE)hInstance)
	{

	}

	PlatformWindows::~PlatformWindows()
	{
		DP_CORE_DEBUG("PLATFORM WINDOWS DELETED!");
	}

	void PlatformWindows::InitializeImpl()
	{
		DP_CORE_INFO("Platform initialized!");

		// Register window class
		WNDCLASSEXW wc = { sizeof(wc)};
		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = m_HInstance;
		

	}

	void PlatformWindows::TerminateImpl()
	{
		DP_CORE_INFO("Platform terminated!");
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (hWnd != nullptr)
		{
			
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}