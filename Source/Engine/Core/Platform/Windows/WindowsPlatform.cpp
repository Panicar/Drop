#include "WindowsPlatform.hpp"
#include "Log/Log.hpp"
#include "WindowsWindow.hpp"

namespace drop
{
	static int CalculateDPI(HMODULE shCoreDll);
	static HMONITOR GetPrimaryMonitorHandle();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	WindowsPlatform::WindowsPlatform(void* hInstance)
		: m_HInstance((HINSTANCE)hInstance), m_DPI(96)
	{

	}

	WindowsPlatform::~WindowsPlatform()
	{
		DP_CORE_DEBUG("PLATFORM WINDOWS DELETED!");
	}

	void WindowsPlatform::InitializeImpl()
	{
		DP_CORE_INFO("Platform initialized!");

		SetHighDPIAwarenessEnabled(true);

		// Register window class
		WNDCLASSEXW wc = { sizeof(wc)};
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = m_HInstance;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.lpszClassName = WindowsWindow::WindowClassName;
		wc.hIcon = (HICON)LoadImageA(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
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

	void* WindowsPlatform::AllocateImpl(size_t size)
	{
		void* ptr = ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, size);
		if (!ptr)
		{
			DP_CORE_ERROR("Failed to allocate memory of size {} bytes!", size);
			return nullptr;
		}
		return ptr;
	}

	void WindowsPlatform::FreeImpl(void* ptr)
	{
		if (ptr)
		{
			if (!::HeapFree(::GetProcessHeap(), 0, ptr))
			{
				DP_CORE_ERROR("Failed to free memory at address {}!", ptr);
			}
		}
	}

	void WindowsPlatform::ZeroMemoryImpl(void* ptr, size_t size)
	{
		if (ptr && size > 0)
		{
			memset(ptr, 0, size);
		}
	}

	void WindowsPlatform::CopyMemoryImpl(void* dest, const void* src, size_t size)
	{
		if (dest && src && size > 0)
		{
			memcpy(dest, src, size);
		}
	}

	void WindowsPlatform::SetMemoryImpl(void* ptr, int value, size_t size)
	{
		if (ptr && size > 0)
		{
			memset(ptr, value, size);
		}
	}

	WCHAR* WindowsPlatform::CreateWideString(const char* str) const
	{
		if (!str)
			return nullptr;
		int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
		if (len <= 0)
			return nullptr;
		WCHAR* wideStr = new WCHAR[len];
		MultiByteToWideChar(CP_UTF8, 0, str, -1, wideStr, len);
		return wideStr;
	}

	std::unique_ptr<WCHAR[]> WindowsPlatform::CreateScopedWideString(const char* str) const
	{
		return std::unique_ptr<WCHAR[]>(CreateWideString(str));	
	}

	void WindowsPlatform::SetHighDPIAwarenessEnabled(bool enabled)
	{
		const HMODULE shCoreDll = LoadLibraryW(L"Shcore.dll");
		if (!shCoreDll)
			return;

		typedef enum _PROCESS_DPI_AWARENESS
		{
			PROCESS_DPI_UNAWARE = 0,
			PROCESS_SYSTEM_DPI_AWARE = 1,
			PROCESS_PER_MONITOR_DPI_AWARE = 2
		} PROCESS_DPI_AWARENESS;
		typedef HRESULT(STDAPICALLTYPE* SetProcessDpiAwarenessProc)(PROCESS_DPI_AWARENESS Value);
		const SetProcessDpiAwarenessProc setProcessDpiAwareness = (SetProcessDpiAwarenessProc)GetProcAddress(shCoreDll, "SetProcessDpiAwareness");
		if (setProcessDpiAwareness)
		{
			setProcessDpiAwareness(enabled ? PROCESS_PER_MONITOR_DPI_AWARE : PROCESS_DPI_UNAWARE);
		}

		m_DPI = CalculateDPI(shCoreDll);
		::FreeLibrary(shCoreDll);
	}

	static int CalculateDPI(HMODULE shCoreDll)
	{
		int dpiX = 96, dpiY = 96;
		if (shCoreDll)
		{
			typedef HRESULT(STDAPICALLTYPE* GetDPIForMonitorProc)(HMONITOR hmonitor, UINT dpiType, UINT* dpiX, UINT* dpiY);
			const GetDPIForMonitorProc getDPIForMonitor = (GetDPIForMonitorProc)GetProcAddress(shCoreDll, "GetDpiForMonitor");

			if (getDPIForMonitor)
			{
				HMONITOR monitor = GetPrimaryMonitorHandle();

				UINT x = 0, y = 0;
				HRESULT hr = getDPIForMonitor(monitor, 0, &x, &y);
				if (SUCCEEDED(hr) && (x > 0) && (y > 0))
				{
					dpiX = (int)x;
					dpiY = (int)y;
				}
			}
		}

		return (dpiX + dpiY) / 2;
	}

	static HMONITOR GetPrimaryMonitorHandle()
	{
		const POINT ptZero = { 0, 0 };
		return MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
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