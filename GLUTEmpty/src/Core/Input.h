#pragma once
#include "Core/KeyCode.h"
#include "Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Core
{
	class Input
	{
	public:
		static void OnActiveMouseMotion(int x, int y);
		static void OnPassiveMouseMotion(int x, int y);
		static void OnMouseButton(int button, int state, int x, int y);
		static void OnKeyPressed(KeyCode key);
		static void OnKeyReleased(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode mouseCode) { return s_ActiveButton != -1 && s_ActiveButton == mouseCode; }
		static bool IsKeyPressed(KeyCode keyCode) { return s_ActiveButton != -1 && s_ActiveKey == keyCode; }
		static glm::vec2 GetMousePosition() { return { (float)s_MouseX, (float)s_MouseY }; }

		static float GetMouseX() { return s_MouseX; }
		static float GetMouseY() { return s_MouseY; }
		static MouseCode GetActiveMouseButton() { return s_ActiveButton; }

	private:
		static int s_MouseX, s_MouseY;
		static MouseCode s_ActiveButton;
		static KeyCode s_ActiveKey;
	};	
}