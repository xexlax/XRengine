#pragma once

#include "pch.h"

#include "xre/Core/Macros.h"
#include "xre/Event/Event.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace XRE {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "XR Engine",
			unsigned int width = WINDOW_WIDTH,
			unsigned int height = WINDOW_HEIGHT)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;
		static Window* Create(const WindowProps& props = WindowProps());

		virtual void HideCursor(bool b)=0;
	};

}