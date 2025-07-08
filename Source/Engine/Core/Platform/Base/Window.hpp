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
	};


	class IWindow
	{
	public:
		virtual ~IWindow() = default;
		virtual void Show() = 0;
		virtual void Update() = 0;

		static IWindow* Create(const WindowProperties& props);

	};

}