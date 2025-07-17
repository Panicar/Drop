#pragma once

#include "Core.hpp"
#include "Platform/Base/Window.hpp"

#include "Input/KeyCodes.hpp"

namespace drop
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const WindowProperties& props);
		~WindowsWindow() override;

		void Show() override;
		LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetWindowHandle() const { return m_WindowHandle; }
	
		void* GetNativeWindowHandle() const override { return static_cast<void*>(m_WindowHandle); }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		bool IsMaximized() const override;
		bool IsMinimized() const override;

		void SetMouseCursor(CursorType type) override;

	public:
		static const WCHAR WindowClassName[];

	private:
		void Initialize();
		void Terminate();
		LPCSTR GetMouseCursorID(CursorType type);
		void UpdateMouseCursor();

		void UpdateModifierKeyStates();

	private:
		WindowProperties m_Properties;
		HINSTANCE m_HInstance = nullptr;
		HWND m_WindowHandle = nullptr;
		int m_Width, m_Height;
		bool m_UsingCustomTitlebar;
	
		// Cursor
		CursorType m_CurrentCursorType = CursorType::Default;
		HCURSOR m_CurrentCursor = nullptr;

		bool m_ModifierKeyState[Modifier::Count];
	};
}