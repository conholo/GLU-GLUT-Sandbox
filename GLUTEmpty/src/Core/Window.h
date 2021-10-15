#pragma once

#include <string>
#include <functional>

#include "Core/Event.h"
#include "Math/Vector.h"

namespace Core
{
	class Window
	{
	public:
		using WindowCallbackFunction = std::function<void(Event&)>;

		static void Initialize(const std::string& name, unsigned int width, unsigned int height, const Math::Vector4& clearColor, const WindowCallbackFunction& callback);

		static unsigned int GetWidth() { return s_Width; }
		static unsigned int GetHeight() { return s_Height; }
		static unsigned int GetMainWindow() { return s_MainWindow; }

		static void OnResize(int width, int height);
		static void OnMouseButton(int button, int state, int x, int y);
		static void OnMouseWheel(int button, int direction, int x, int y);
		static void OnMouseActiveMotion(int x, int y);
		static void OnMousePassiveMotion(int x, int y);
		static void OnKeyboardDown(unsigned char key, int x, int y);
		static void OnKeyboardUp(unsigned char key, int x, int y);
		static void OnDisplay();
		static void OnIdle();

	private:
		static bool s_Initialized;
		static WindowCallbackFunction s_EventCallbackFunction;
		static unsigned int s_MainWindow;
		static unsigned int s_Width, s_Height;
	};
}