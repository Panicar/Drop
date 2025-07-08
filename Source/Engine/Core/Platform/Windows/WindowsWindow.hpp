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
		HWND GetWindowHandle() const { return m_WindowHandle; }
	
	public:
		static const WCHAR WindowClassName[];

	private:
		void Initialize();
		void Terminate();

	private:
		HWND m_WindowHandle = nullptr;
		HINSTANCE m_HInstance = nullptr;
	};
}