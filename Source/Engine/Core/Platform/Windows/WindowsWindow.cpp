#include "WindowsWindow.hpp"
#include "Platform/Base/Platform.hpp"
#include "WindowsPlatform.hpp"

#include <WinUser.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#ifndef GET_X_LPARAM  
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))  
#endif  

#ifndef GET_Y_LPARAM  
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))  
#endif

#define TITLEBAR_HEIGHT 40

namespace drop
{

	static int GetModifierIdx(Modifier mod)
	{
		switch (mod)
		{
			case Modifier::None: return 0;
			case Modifier::LeftShift: return 1;
			case Modifier::RightShift: return 2;
			case Modifier::LeftControl: return 3;
			case Modifier::RightControl: return 4;
			case Modifier::LeftAlt: return 5;
			case Modifier::RightAlt: return 6;
			case Modifier::CapsLock: return 7;
		}
	}

	static Key TranslateVkToKey(WPARAM vk, LPARAM lParam)
	{
		switch (vk)
		{
			// Printable keys (ASCII-matching)
		case VK_SPACE: return Key::Space;
		case 0x30: return Key::Num0;
		case 0x31: return Key::Num1;
		case 0x32: return Key::Num2;
		case 0x33: return Key::Num3;
		case 0x34: return Key::Num4;
		case 0x35: return Key::Num5;
		case 0x36: return Key::Num6;
		case 0x37: return Key::Num7;
		case 0x38: return Key::Num8;
		case 0x39: return Key::Num9;

		case 'A': return Key::A;
		case 'B': return Key::B;
		case 'C': return Key::C;
		case 'D': return Key::D;
		case 'E': return Key::E;
		case 'F': return Key::F;
		case 'G': return Key::G;
		case 'H': return Key::H;
		case 'I': return Key::I;
		case 'J': return Key::J;
		case 'K': return Key::K;
		case 'L': return Key::L;
		case 'M': return Key::M;
		case 'N': return Key::N;
		case 'O': return Key::O;
		case 'P': return Key::P;
		case 'Q': return Key::Q;
		case 'R': return Key::R;
		case 'S': return Key::S;
		case 'T': return Key::T;
		case 'U': return Key::U;
		case 'V': return Key::V;
		case 'W': return Key::W;
		case 'X': return Key::X;
		case 'Y': return Key::Y;
		case 'Z': return Key::Z;

			// Symbols
		case VK_OEM_1: return Key::Semicolon;       // ;:
		case VK_OEM_PLUS: return Key::Equal;        // =+
		case VK_OEM_COMMA: return Key::Comma;       // ,<
		case VK_OEM_MINUS: return Key::Minus;       // -_
		case VK_OEM_PERIOD: return Key::Period;     // .>
		case VK_OEM_2: return Key::Slash;           // /?
		case VK_OEM_3: return Key::GraveAccent;     // `~
		case VK_OEM_4: return Key::LeftBracket;     // [{
		case VK_OEM_5: return Key::BackSlash;       // \|
		case VK_OEM_6: return Key::RightBracket;    // ]}
		case VK_OEM_7: return Key::Apostrophe;      // '"

			// Function keys
		case VK_ESCAPE: return Key::Escape;
		case VK_RETURN:
		{
			if (HIWORD(lParam) & KF_EXTENDED)
				return Key::KPEnter;
			return Key::Enter;
		}
		case VK_TAB: return Key::Tab;
		case VK_BACK: return Key::BackSpace;
		case VK_INSERT: return Key::Insert;
		case VK_DELETE: return Key::Delete;
		case VK_RIGHT: return Key::Right;
		case VK_LEFT: return Key::Left;
		case VK_DOWN: return Key::Down;
		case VK_UP: return Key::Up;
		case VK_PRIOR: return Key::PageUp;
		case VK_NEXT: return Key::PageDown;
		case VK_HOME: return Key::Home;
		case VK_END: return Key::End;
		case VK_CAPITAL: return Key::CapsLock;
		case VK_SCROLL: return Key::ScrollLock;
		case VK_NUMLOCK: return Key::NumLock;
		case VK_SNAPSHOT: return Key::PrintSCREEN;
		case VK_PAUSE: return Key::Pause;

			// F1–F25
		case VK_F1: return Key::F1;
		case VK_F2: return Key::F2;
		case VK_F3: return Key::F3;
		case VK_F4: return Key::F4;
		case VK_F5: return Key::F5;
		case VK_F6: return Key::F6;
		case VK_F7: return Key::F7;
		case VK_F8: return Key::F8;
		case VK_F9: return Key::F9;
		case VK_F10: return Key::F10;
		case VK_F11: return Key::F11;
		case VK_F12: return Key::F12;
		case VK_F13: return Key::F13;
		case VK_F14: return Key::F14;
		case VK_F15: return Key::F15;
		case VK_F16: return Key::F16;
		case VK_F17: return Key::F17;
		case VK_F18: return Key::F18;
		case VK_F19: return Key::F19;
		case VK_F20: return Key::F20;
		case VK_F21: return Key::F21;
		case VK_F22: return Key::F22;
		case VK_F23: return Key::F23;
		case VK_F24: return Key::F24;

			// Keypad
		case VK_NUMPAD0: return Key::KP0;
		case VK_NUMPAD1: return Key::KP1;
		case VK_NUMPAD2: return Key::KP2;
		case VK_NUMPAD3: return Key::KP3;
		case VK_NUMPAD4: return Key::KP4;
		case VK_NUMPAD5: return Key::KP5;
		case VK_NUMPAD6: return Key::KP6;
		case VK_NUMPAD7: return Key::KP7;
		case VK_NUMPAD8: return Key::KP8;
		case VK_NUMPAD9: return Key::KP9;
		case VK_DECIMAL: return Key::KPDecimal;
		case VK_DIVIDE: return Key::KPDivide;
		case VK_MULTIPLY: return Key::KPMultiply;
		case VK_SUBTRACT: return Key::KPSubtract;
		case VK_ADD: return Key::KPAdd;
		case VK_SEPARATOR: return Key::KPEqual;
		
			// Modifiers
		case VK_LSHIFT: return Key::LeftShift;
		case VK_RSHIFT: return Key::RightShift;
		case VK_LCONTROL: return Key::LeftControl;
		case VK_RCONTROL: return Key::RightControl;
		case VK_LMENU: return Key::LeftAlt;
		case VK_RMENU: return Key::RightAlt;
		case VK_LWIN: return Key::LeftSuper;
		case VK_RWIN: return Key::RightSuper;
		case VK_APPS: return Key::Menu;

		default: return Key::Unknown;
		}
	}
	const WCHAR WindowsWindow::WindowClassName[] = L"DropWindow";

	WindowsWindow::WindowsWindow(const WindowProperties& props)
		: m_Properties(props)
	{
		m_HInstance = IPlatform::As<WindowsPlatform>()->GetHInstance();
		m_UsingCustomTitlebar = !props.DefaultTitlebar;
		m_Callbacks = props.Callbacks;
		Initialize();
	}

	WindowsWindow::~WindowsWindow()
	{
		Terminate();
		m_WindowHandle = nullptr;
		
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
		
		m_Width = width;
		m_Height = height;

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
	}

	LPCTSTR WindowsWindow::GetMouseCursorID(CursorType type)
	{
		switch (type)
		{
			case CursorType::Default:      return IDC_ARROW;
			case CursorType::Arrow:        return IDC_ARROW;
			case CursorType::IBeam:        return IDC_IBEAM;
			case CursorType::Wait:         return IDC_WAIT;
			case CursorType::Cross:        return IDC_CROSS;
			case CursorType::UpArrow:      return IDC_UPARROW;
			case CursorType::Size:         return IDC_SIZE;
			case CursorType::Icon:         return IDC_ICON;
			case CursorType::SizeNWSE:     return IDC_SIZENWSE;
			case CursorType::SizeNESW:     return IDC_SIZENESW;
			case CursorType::SizeWE:       return IDC_SIZEWE;
			case CursorType::SizeNS:       return IDC_SIZENS;
			case CursorType::SizeAll:      return IDC_SIZEALL;
			case CursorType::No:           return IDC_NO;
			case CursorType::Hand:         return IDC_HAND;
			case CursorType::AppStarting:  return IDC_APPSTARTING;
			case CursorType::Help:         return IDC_HELP;
			case CursorType::Hidden:       return nullptr; // Special case: hide the cursor
			default:                       return IDC_ARROW;
		}
	}

	void WindowsWindow::SetMouseCursor(CursorType type)
	{
		if (m_CurrentCursorType != type)
		{
			m_CurrentCursorType = type;
			m_CurrentCursor = nullptr;
		}
	}

	void WindowsWindow::UpdateMouseCursor()
	{
		if (m_CurrentCursorType == CursorType::Hidden)
		{
			::SetCursor(nullptr);
			return;
		}

		if (!m_CurrentCursor)
		{
			LPCTSTR cursorID = GetMouseCursorID(m_CurrentCursorType);
			m_CurrentCursor = ::LoadCursor(nullptr, cursorID);
		}

		if (m_CurrentCursor)
		{
			::SetCursor(m_CurrentCursor);
		}
	}

	void WindowsWindow::UpdateModifierKeyStates()
	{
		m_ModifierKeyState[GetModifierIdx(Modifier::LeftShift)] = (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0;
		m_ModifierKeyState[GetModifierIdx(Modifier::RightShift)] = (GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;
		m_ModifierKeyState[GetModifierIdx(Modifier::LeftControl)] = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0;
		m_ModifierKeyState[GetModifierIdx(Modifier::RightControl)] = (GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0;
		m_ModifierKeyState[GetModifierIdx(Modifier::LeftAlt)] = (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0;
		m_ModifierKeyState[GetModifierIdx(Modifier::RightAlt)] = (GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
		m_ModifierKeyState[GetModifierIdx(Modifier::CapsLock)] = (GetAsyncKeyState(VK_CAPITAL) & 0x8000) != 0;
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
			{
				return 0;
			}
			case WM_CLOSE:
			{
				if(m_Properties.Callbacks.Close)
				{
					m_Properties.Callbacks.Close();
				}
				
				DestroyWindow(m_WindowHandle);
				break;
			}
		
			case WM_SIZE:
			{
				const int width = LOWORD(lParam);
				const int height = HIWORD(lParam);
				const bool maximized = (wParam == SIZE_MAXIMIZED);
				const bool minimized = (wParam == SIZE_MINIMIZED);

				// handle minimize, maximize

				if (width != m_Width || height != m_Height)
				{
					m_Width = width;
					m_Height = height;
					
					if (m_Callbacks.Resize)
						m_Callbacks.Resize(width, height);
				}

				break;
			}

			case WM_PAINT:
			{
				/*if (!m_UsingCustomTitlebar)
					break;*/

				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_WindowHandle, &ps);

				// Render custom titlebar and client area here
				// Example: Fill with different color for titlebar area
				RECT titlebarRect = ps.rcPaint;
				HBRUSH titleBrush = CreateSolidBrush(RGB(10, 10, 10));
				titlebarRect.bottom = TITLEBAR_HEIGHT; // Titlebar height
				FillRect(hdc, &titlebarRect, titleBrush);
				DeleteObject(titleBrush);

				RECT clientRect = ps.rcPaint;
				HBRUSH clientBrush = CreateSolidBrush(RGB(26, 26, 26));
				clientRect.top = TITLEBAR_HEIGHT;
				FillRect(hdc, &clientRect, clientBrush);
				DeleteObject(clientBrush);


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
			// TODO: Handle custom titlebar buttons (close, minimize, maximize)
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

				const UINT dpi = GetDpiForWindow(m_WindowHandle);
				const int frameY = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
				const int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
				POINT cursorPoint = { 0 };
				cursorPoint.x = GET_X_LPARAM(lParam);
				cursorPoint.y = GET_Y_LPARAM(lParam);
				ScreenToClient(m_WindowHandle, &cursorPoint);
				
				if (!IsMaximized() && cursorPoint.y > 0 && cursorPoint.y < frameY + padding)
				{
					return HTTOP;
				}

				if (cursorPoint.y <= TITLEBAR_HEIGHT) {
					return HTCAPTION;
				}

				return HTCLIENT;

			}

			case WM_SETCURSOR:
			{
				if(LOWORD(lParam) == HTCLIENT)
				{
					UpdateMouseCursor();
					return 1;
				}

				break;
			}

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				UpdateModifierKeyStates();
				const int vk = static_cast<int>(wParam);

				if (vk == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x8000)) // Check if Alt + F4 is pressed
				{
					return DefWindowProcW(m_WindowHandle, msg, wParam, lParam); // Let the OS handle it
				}

				const Action action = (HIWORD(lParam) & KF_UP) ? Action::Release : Action::Press;
				int keyCode = vk;
				bool isRepeat = (lParam & 0x40000000) != 0;
				int scanCode = (lParam & 0x00ff0000) >> 16;
				bool isExtended = (lParam & 0x1000000) != 0;

				switch (keyCode)
				{
				case VK_MENU:
				{
					if (isExtended)
					{
						keyCode = VK_RMENU;
						isRepeat = m_ModifierKeyState[GetModifierIdx(Modifier::RightAlt)];
						m_ModifierKeyState[GetModifierIdx(Modifier::RightAlt)] = (action == Action::Press);
					}
					else
					{
						keyCode = VK_LMENU;
						isRepeat = m_ModifierKeyState[GetModifierIdx(Modifier::LeftAlt)];
						m_ModifierKeyState[GetModifierIdx(Modifier::LeftAlt)] = (action == Action::Press);
					}

					break;
				}

				case VK_CONTROL:
				{
					if (isExtended)
					{
						keyCode = VK_RCONTROL;
						isRepeat = m_ModifierKeyState[GetModifierIdx(Modifier::RightControl)];
						m_ModifierKeyState[GetModifierIdx(Modifier::RightControl)] = (action == Action::Press);
					}
					else
					{
						keyCode = VK_LCONTROL;
						isRepeat = m_ModifierKeyState[GetModifierIdx(Modifier::LeftControl)];
						m_ModifierKeyState[GetModifierIdx(Modifier::LeftControl)] = (action == Action::Press);
					}

					break;
				}

				case VK_SHIFT:
				{
					keyCode = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);

					if (keyCode == VK_RSHIFT)
					{
						isRepeat = m_ModifierKeyState[GetModifierIdx(Modifier::RightShift)];
						m_ModifierKeyState[GetModifierIdx(Modifier::RightShift)] = (action == Action::Press);
					}
					else
					{
						isRepeat = m_ModifierKeyState[GetModifierIdx(Modifier::LeftShift)];
						m_ModifierKeyState[GetModifierIdx(Modifier::LeftShift)] = (action == Action::Press);
					}

					break;
				}
				case VK_CAPITAL:
				{
					m_ModifierKeyState[GetModifierIdx(Modifier::CapsLock)] = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
					break;
				}
				default:
					break;
				}

				uint32_t charCode = MapVirtualKey(keyCode, MAPVK_VK_TO_CHAR);

				Modifier mods = Modifier::None;

				using U = std::underlying_type_t<Modifier>;
				U rawMods = static_cast<U>(mods);

				if (m_ModifierKeyState[GetModifierIdx(Modifier::LeftShift)])
					rawMods |= static_cast<U>(Modifier::LeftShift);

				if (m_ModifierKeyState[GetModifierIdx(Modifier::RightShift)])
					rawMods |= static_cast<U>(Modifier::RightShift);
				
				if (m_ModifierKeyState[GetModifierIdx(Modifier::LeftControl)])
					rawMods |= static_cast<U>(Modifier::LeftControl);

				if (m_ModifierKeyState[GetModifierIdx(Modifier::RightControl)])
					rawMods |= static_cast<U>(Modifier::RightControl);

				if (m_ModifierKeyState[GetModifierIdx(Modifier::LeftAlt)])
					rawMods |= static_cast<U>(Modifier::LeftAlt);

				if (m_ModifierKeyState[GetModifierIdx(Modifier::RightAlt)])
					rawMods |= static_cast<U>(Modifier::RightAlt);

				if (m_ModifierKeyState[GetModifierIdx(Modifier::CapsLock)])
					rawMods |= static_cast<U>(Modifier::CapsLock);

				mods = static_cast<Modifier>(rawMods);

				if (m_Callbacks.Key)
				{
					bool res = m_Callbacks.Key(TranslateVkToKey(keyCode, lParam), charCode, action, isRepeat, mods);
					
					if (res && msg != WM_KEYDOWN && msg != WM_SYSKEYUP)
						return 0;
				
				}
					
				break;
			}

			case WM_SYSCHAR:
			case WM_CHAR:
			{
				
				break;
			}

			case WM_ERASEBKGND:
			{
				// Prevent flickering by not erasing the background
				return 1;
			}


		}
		return DefWindowProcW(m_WindowHandle, msg, wParam, lParam);
	}
}