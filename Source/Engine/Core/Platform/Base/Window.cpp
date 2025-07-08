#include "Window.hpp"
#include "Core.hpp"

#ifdef DP_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.hpp"
using Window = drop::WindowsWindow;
#elif DP_PLATFORM_LINUX
#include "Platform/Linux/WindowLinux.hpp"
using Window = drop::WindowLinux;
#endif

namespace drop
{
	IWindow* IWindow::Create(const WindowProperties& props)
	{
		return new Window(props);
	}
}