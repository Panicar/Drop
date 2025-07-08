#include "WindowsWindow.hpp"
#include "Platform/Base/Platform.hpp"
#include "WindowsPlatform.hpp"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

namespace drop
{
	const WCHAR WindowsWindow::WindowClassName[] = L"DropWindow";

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		m_HInstance = IPlatform::As<WindowsPlatform>()->GetHInstance();
		Initialize();
	}

	WindowsWindow::~WindowsWindow()
	{
		if (m_WindowHandle)
		{
			Terminate();
			m_WindowHandle = nullptr;
		}
		else
		{
			DP_CORE_ERROR("Window handle is null, cannot destroy window!");
		}
		DP_CORE_INFO("Windows Windows destroyed!");
	}

	void WindowsWindow::Show()
	{
		if (!m_WindowHandle)
		{
			DP_CORE_ERROR("Window handle is null!");
			return;
		}
		ShowWindow(m_WindowHandle, SW_SHOW);
		UpdateWindow(m_WindowHandle);
	}

	void WindowsWindow::Initialize()
	{
		DP_CORE_INFO("Windows Window initialized!");
		m_WindowHandle = CreateWindowExW(
			0,
			WindowClassName,
			L"Drop Engine",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
			nullptr, nullptr, m_HInstance, this);
	
		if (!m_WindowHandle)
		{
			DWORD err = GetLastError();
			DP_CORE_ERROR("Failed to create window! Error code: {}", err);
			return;
		}
		DP_CORE_INFO("Window created successfully!");

		// Testing
		uint32_t captionColor = RGB(15, 15, 15);
		DwmSetWindowAttribute(m_WindowHandle, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));

		uint32_t borderColor = RGB(35, 84, 156);
		DwmSetWindowAttribute(m_WindowHandle, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));

		SetWindowPos(m_WindowHandle, nullptr, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

		Show();

	}

	void WindowsWindow::Terminate()
	{
		if (m_WindowHandle)
		{
			DestroyWindow(m_WindowHandle);
			m_WindowHandle = nullptr;
			DP_CORE_INFO("Window destroyed successfully!");
		}
		else
		{
			DP_CORE_ERROR("Window handle is null, cannot destroy window!");
		}
	}

	LRESULT WindowsWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_CLOSE:
				DestroyWindow(m_WindowHandle);
				break;
		
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_WindowHandle, &ps);
				HBRUSH brush = CreateSolidBrush(RGB(26, 26, 26));
				FillRect(hdc, &ps.rcPaint, brush);
				DeleteObject(brush);
				EndPaint(m_WindowHandle, &ps);
		
				return 0;
			}
		
		}
		return DefWindowProcW(m_WindowHandle, msg, wParam, lParam);
	}

	void WindowsWindow::Update()
	{
		MSG msg;
		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

}