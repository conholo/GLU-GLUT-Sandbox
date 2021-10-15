#include "Core/Input.h"
#include "Core/Window.h"

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include <iostream>

namespace Core
{
	int Input::s_MouseX = 0;
	int Input::s_MouseY = 0;
	MouseCode Input::s_ActiveButton = -1;
	KeyCode Input::s_ActiveKey = -1;

	void Input::OnPassiveMouseMotion(int x, int y)
	{
		s_MouseX = x;
		s_MouseY = y;
	}

	void Input::OnActiveMouseMotion(int x, int y)
	{
		int deltaX = x - s_MouseX;
		int deltaY = y - s_MouseY;

		s_MouseX = x;
		s_MouseY = y;

		glutSetWindow(Window::GetMainWindow());
		glutPostRedisplay();
	}

	void Input::OnMouseButton(int button, int state, int x, int y)
	{
		MouseCode thisFrameButton = (MouseCode)button;

		s_ActiveButton = state == GLUT_DOWN ? thisFrameButton : -1;

		glutSetWindow(Window::GetMainWindow());
		glutPostRedisplay();
	}

	void Input::OnKeyPressed(KeyCode key)
	{
		s_ActiveKey = key;
		glutSetWindow(Window::GetMainWindow());
		glutPostRedisplay();
	}

	void Input::OnKeyReleased(KeyCode key)
	{
		s_ActiveKey = -1;
		glutSetWindow(Window::GetMainWindow());
		glutPostRedisplay();
	}
}