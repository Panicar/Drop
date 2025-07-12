#pragma once

#include "Core.hpp"
#include "Platform/Base/Window.hpp"


namespace drop
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const WindowProperties& props);
		~WindowsWindow() override;

		void Show() override;
		void Update() override;
		LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleClientMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetWindowHandle() const { return m_WindowHandle; }
	
		void* GetNativeWindowHandle() const override { return static_cast<void*>(m_WindowHandle); }
		bool IsMaximized() const override;
		bool IsMinimized() const override;

	public:
		static const WCHAR WindowClassName[];
		static const WCHAR ClientWindowClassName[];

	private:
		void Initialize();
		void Terminate();

	private:
		WindowProperties m_Properties;
		HINSTANCE m_HInstance = nullptr;
		HWND m_WindowHandle = nullptr;
		HWND m_ClientWindowHandle = nullptr;
		bool m_UsingCustomTitlebar;
	};
}