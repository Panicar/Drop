#pragma once 

#include <string>
#include <memory>

namespace drop
{
	struct WindowProperties
	{
		std::string Title = "Untitled";
		int Width = 1280;
		int Height = 720;
		int PositionX = 100;
		int PositionY = 100;

		bool DefaultTitlebar = true;
		
		bool IsBorderless = false;
		bool SupportsMaximize = true;
		bool SupportsMinimize = true;
		bool IsResizable = true;
		//bool IsFullscreen = false;
		//bool IsVSyncEnabled = true;
		bool IsMaximized = false;
		bool IsMinimized = false;
	};

	class IWindow
	{
	public:
		virtual ~IWindow() = default;
		virtual void Show() = 0;
		virtual void Update() = 0;

		virtual void* GetNativeWindowHandle() const = 0;

		virtual bool IsMaximized() const = 0;
		virtual bool IsMinimized() const = 0;

		static IWindow* Create(const WindowProperties& props);

	};

}