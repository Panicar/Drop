#pragma once 

#include "Input/KeyCodes.hpp"

#include <string>
#include <memory>
#include <functional>


namespace drop
{
	// Callbacks
	using WindowCloseCallback = std::function<void()>;
	using WindowResizeCallback = std::function<void(int width, int height)>;

	using KeyCallback = std::function<bool(Key key, char charCode, Action action, bool isRepeat, Modifier mods)>;

	struct CallbackFunctions
	{
		WindowCloseCallback Close = nullptr;
		WindowResizeCallback Resize = nullptr;
		KeyCallback Key = nullptr;
	};

	struct WindowProperties
	{
		CallbackFunctions Callbacks = {};
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

	enum class CursorType 
	{
		Default = 0,
		Arrow,          // IDC_ARROW
		IBeam,          // IDC_IBEAM
		Wait,           // IDC_WAIT
		Cross,          // IDC_CROSS
		UpArrow,        // IDC_UPARROW
		Size,           // IDC_SIZE (legacy)
		Icon,           // IDC_ICON (legacy)
		SizeNWSE,       // IDC_SIZENWSE
		SizeNESW,       // IDC_SIZENESW
		SizeWE,         // IDC_SIZEWE
		SizeNS,         // IDC_SIZENS
		SizeAll,        // IDC_SIZEALL
		No,             // IDC_NO
		Hand,           // IDC_HAND
		AppStarting,    // IDC_APPSTARTING
		Help,           // IDC_HELP
		Hidden,         // IDC_ARROW (hidden cursor)
	};

	class IWindow
	{
	public:
		virtual ~IWindow() = default;
		virtual void Show() = 0;

		virtual void* GetNativeWindowHandle() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual bool IsMaximized() const = 0;
		virtual bool IsMinimized() const = 0;

		virtual void SetMouseCursor(CursorType cursorType) = 0;

		void SetWindowCloseCallback(const WindowCloseCallback& callback) { m_Callbacks.Close = callback; }
		void SetWindowResizeCallback(const WindowResizeCallback& callback) { m_Callbacks.Resize = callback; }
		void SetKeyCallback(const KeyCallback& callback) { m_Callbacks.Key = callback; }

		static IWindow* Create(const WindowProperties& props);

		const CallbackFunctions& GetCallbacks() const { return m_Callbacks; }
			CallbackFunctions& GetCallbacks() { return m_Callbacks; }


	protected:
		CallbackFunctions m_Callbacks;

	};

}