#include "WindowsWindow.hpp"
#include "Platform/Base/Platform.hpp"
#include "WindowsPlatform.hpp"

#include <WinUser.h>
#include <ShellScalingApi.h>
#pragma comment(lib, "Shcore.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")


#include <uxtheme.h>
#include <vssym32.h>

#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")


#ifndef GET_X_LPARAM  
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))  
#endif  

#ifndef GET_Y_LPARAM  
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))  
#endif

#define CUSTOM_TITLEBAR_HEIGHT 40

namespace drop
{
	const WCHAR WindowsWindow::WindowClassName[] = L"DropWindow";
	const WCHAR WindowsWindow::ClientWindowClassName[] = L"DropClientWindow";

	WindowsWindow::WindowsWindow(const WindowProperties& props)
		: m_Properties(props)
	{
		m_HInstance = IPlatform::As<WindowsPlatform>()->GetHInstance();
		m_UsingCustomTitlebar = !props.DefaultTitlebar;
		Initialize();
	}

	WindowsWindow::~WindowsWindow()
	{
		Terminate();
		m_WindowHandle = nullptr;
		m_ClientWindowHandle = nullptr;
		
		DP_CORE_INFO("Windows Windows destroyed!");
	}

	void WindowsWindow::Initialize()
	{
		DP_CORE_INFO("Windows Window initialized!");
		int width = m_Properties.Width;
		int height = m_Properties.Height;
		int windowX = m_Properties.PositionX;
		int windowY = m_Properties.PositionY;
		uint32_t windowStyle = 0;
		uint32_t windowExStyle = 0;
		
		if (m_Properties.IsBorderless)
		{
			// TODO: Handle borderless window
			windowStyle = WS_POPUPWINDOW;
			//windowExStyle |= WS_EX_TOOLWINDOW;
		}
		else
		{
			windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
			windowExStyle = WS_EX_APPWINDOW;

			if (m_Properties.SupportsMaximize)
			{
				windowStyle |= WS_MAXIMIZEBOX;
			}

			if (m_Properties.SupportsMinimize)
			{
				windowStyle |= WS_MINIMIZEBOX;
			}

			if (m_Properties.IsResizable)
			{
				windowStyle |= WS_THICKFRAME;
			}
			else
			{
				windowStyle |= WS_BORDER;
			}
			
			if (m_Properties.DefaultTitlebar)
			{
				RECT borderRect = { 0, 0, 0, 0 };
				AdjustWindowRectEx(&borderRect, windowStyle, FALSE, windowExStyle);

				windowX += borderRect.left;
				windowY += borderRect.top;
				width += (borderRect.right - borderRect.left);
				height += (borderRect.bottom - borderRect.top);

				DP_CORE_INFO("Window Rect: left: {}, top: {}, right: {}, bottom: {}",
					borderRect.left, borderRect.top, borderRect.right, borderRect.bottom);

			}
			else
			{
				// Left Top Right Bottom
				RECT borderRect = { 0, 0, 0, 0 };
				AdjustWindowRectEx(&borderRect, windowStyle, FALSE, windowExStyle);

				DP_CORE_INFO("Window Rect: left: {}, top: {}, right: {}, bottom: {}",
					borderRect.left, borderRect.top, borderRect.right, borderRect.bottom);

				windowX += borderRect.left;
				windowY += -1;
				width += (borderRect.right - borderRect.left);
				height += (borderRect.bottom + 1);
			}
		}

		WCHAR* wideTitle = IPlatform::As<WindowsPlatform>()->CreateWideString(m_Properties.Title.c_str());

		DP_CORE_INFO("windowX: {}, windowY: {}, width: {}, height: {}", windowX, windowY, width, height);

		m_WindowHandle = CreateWindowExW(
			windowExStyle,
			WindowClassName,
			wideTitle,
			windowStyle,
			windowX, windowY, width, height,
			nullptr, nullptr, m_HInstance, this);
	
		delete[] wideTitle;

		if (!m_WindowHandle)
		{
			DWORD err = GetLastError();
			DP_CORE_ERROR("Failed to create window! Error code: {}", err);
			return;
		}
		DP_CORE_INFO("Window created successfully!");

		if (m_UsingCustomTitlebar)
		{
			m_ClientWindowHandle = CreateWindowExW(
				0,
				ClientWindowClassName,
				L"",
				WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
				0, 1, width, height - 1,
				m_WindowHandle,
				nullptr,
				m_HInstance,
				this
			);

			if (!m_ClientWindowHandle) 
			{
				DP_CORE_ERROR("Failed to create client window");
			}
			else 
			{
				SetWindowPos(m_ClientWindowHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				UpdateWindow(m_ClientWindowHandle);
				DP_CORE_INFO("Client window created successfully!");
			}

		}

		// Testing
		const uint32_t captionColor = RGB(15, 15, 15);
		DwmSetWindowAttribute(m_WindowHandle, DWMWA_CAPTION_COLOR, &captionColor, sizeof(captionColor));

		const uint32_t borderColor = RGB(35, 84, 156);
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
		
		if (m_ClientWindowHandle)
		{
			DestroyWindow(m_ClientWindowHandle);
			m_ClientWindowHandle = nullptr;
			DP_CORE_INFO("Client window destroyed successfully!");
		}

	}

    bool WindowsWindow::IsMaximized() const  
    {  
        if (!m_WindowHandle)  
        {  
            DP_CORE_ERROR("Window handle is null!");  
            return false;  
        }  
    
		return IsZoomed(m_WindowHandle) != 0;  
    }

	bool WindowsWindow::IsMinimized() const
	{
		if (!m_WindowHandle)
		{
			DP_CORE_ERROR("Window handle is null!");
			return false;
		}

		return IsIconic(m_WindowHandle) != 0;
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
		
			case WM_SIZE:
			{
				if (m_ClientWindowHandle)
				{
					// Resize client window to fill main window's client area
					RECT rc;
					GetClientRect(m_WindowHandle, &rc);
					SetWindowPos(m_ClientWindowHandle, nullptr,
						0, 1, rc.right, rc.bottom - 1,
						SWP_NOZORDER | SWP_NOACTIVATE);
				}
				break;
			}

			case WM_PAINT:
			{
				if(m_UsingCustomTitlebar)
					break;

				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_WindowHandle, &ps);
				HBRUSH brush = CreateSolidBrush(RGB(126, 26, 26));
				FillRect(hdc, &ps.rcPaint, brush);
				DeleteObject(brush);
				EndPaint(m_WindowHandle, &ps);
		
				return 0;
			}

#if 1
			case WM_NCCALCSIZE:
			{
				if (!wParam || !m_UsingCustomTitlebar)
					break;

				const uint32_t dpi = GetDpiForWindow(m_WindowHandle);
				const int resizeBorderX = GetSystemMetricsForDpi(SM_CXFRAME, dpi);
				const int resizeBorderY = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
				const int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);	

				NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lParam;
				RECT* requestedClientRect = params->rgrc;

				requestedClientRect->right -= (resizeBorderX + padding);
				requestedClientRect->left += (resizeBorderX + padding);
				requestedClientRect->bottom -= (resizeBorderY + padding);
				requestedClientRect->top += 0;

				if (IsMaximized())
				{
					requestedClientRect->top += padding;
				}

				return 0;
			}
#endif

			case WM_NCHITTEST:
			{
				if (!m_UsingCustomTitlebar)
					break;

				LRESULT hit = DefWindowProc(m_WindowHandle, msg, wParam, lParam);
				switch (hit)
				{
					case HTNOWHERE:
					case HTRIGHT:
					case HTLEFT:
					case HTTOPLEFT:
					case HTTOP:
					case HTTOPRIGHT:
					case HTBOTTOMRIGHT:
					case HTBOTTOM:
					case HTBOTTOMLEFT:
					{
						return hit;
					}
				}

				UINT dpi = GetDpiForWindow(m_WindowHandle);
				int frameY = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
				int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
				POINT cursorPoint = { 0 };
				cursorPoint.x = GET_X_LPARAM(lParam);
				cursorPoint.y = GET_Y_LPARAM(lParam);
				ScreenToClient(m_WindowHandle, &cursorPoint);
				
				if (!IsMaximized() && cursorPoint.y > 0 && cursorPoint.y < frameY + padding)
				{
					return HTTOP;
				}

				if (cursorPoint.y <= CUSTOM_TITLEBAR_HEIGHT) {
					return HTCAPTION;
				}

				return HTCLIENT;

			}

		}
		return DefWindowProcW(m_WindowHandle, msg, wParam, lParam);
	}

	LRESULT WindowsWindow::HandleClientMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_ClientWindowHandle, &ps);

				// Render custom titlebar and client area here
				// Example: Fill with different color for titlebar area
				RECT titlebarRect = ps.rcPaint;
				HBRUSH titleBrush = CreateSolidBrush(RGB(10, 10, 10));
				titlebarRect.bottom = CUSTOM_TITLEBAR_HEIGHT; // Titlebar height
				FillRect(hdc, &titlebarRect, titleBrush);
				DeleteObject(titleBrush);

				RECT clientRect = ps.rcPaint;	
				HBRUSH clientBrush = CreateSolidBrush(RGB(26, 26, 26));
				clientRect.top = CUSTOM_TITLEBAR_HEIGHT;
				FillRect(hdc, &clientRect, clientBrush);
				DeleteObject(clientBrush);
			

				EndPaint(m_ClientWindowHandle, &ps);
				return 0;
			}

			case WM_NCHITTEST:
			{
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				ScreenToClient(m_ClientWindowHandle, &pt);

				if (pt.y < CUSTOM_TITLEBAR_HEIGHT)
				{
					return HTTRANSPARENT;
				}
				return HTCLIENT;
			}

		}
		
		return DefWindowProcW(m_ClientWindowHandle, msg, wParam, lParam);
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