#include "WindowsPlatform.hpp"
#include "Log/Log.hpp"
#include "WindowsWindow.hpp"

#include <ShellScalingApi.h>
#pragma comment(lib, "Shcore.lib")

namespace drop
{
	
	class WindowsWindow; // Forward declaration

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	WindowsPlatform::WindowsPlatform(void* hInstance)
		: m_HInstance((HINSTANCE)hInstance)
	{

	}

	WindowsPlatform::~WindowsPlatform()
	{
		DP_CORE_DEBUG("PLATFORM WINDOWS DELETED!");
	}

	void WindowsPlatform::InitializeImpl()
	{
		DP_CORE_INFO("Platform initialized!");

		SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

		// Register window class
		WNDCLASSEXW wc = { sizeof(wc)};
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = m_HInstance;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.lpszClassName = WindowsWindow::WindowClassName;
		wc.hIcon = (HICON)LoadImageW(nullptr, L"Drop Icon", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

		if (!RegisterClassExW(&wc))
		{
			DP_CORE_ERROR("Window class registration fialed!");
			__debugbreak();
		}

		DP_CORE_INFO("Window class registered!");
	}

	void WindowsPlatform::TerminateImpl()
	{
		DP_CORE_INFO("Platform terminated!");
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		/* We use WM_NCCREATE because it is the first message sent to a window 
		where we have access to the lpCreateParams pointer — which contains 
		the 'this' pointer we passed during CreateWindowEx. */

		if (msg == WM_NCCREATE)
		{
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);	
			//DP_CORE_INFO("WM_NCCREATE received. lpCreateParams = {}", (void*)cs->lpCreateParams);
			auto* window = static_cast<drop::WindowsWindow*>(cs->lpCreateParams);
			
			if (window)
			{
				DP_CORE_INFO("Window pointer is valid, setting user data.");
				SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
				
				return DefWindowProcW(hWnd, msg, wParam, lParam);
			}
			else
			{
				DP_CORE_ERROR("Window pointer is null in WM_NCCREATE!");
				return FALSE;
			}
			
		}

		auto* window = reinterpret_cast<drop::WindowsWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		if (window)
		{
			//DP_CORE_INFO("We have a window pointer!");
			return window->HandleMessage(msg, wParam, lParam);
		}


		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

}