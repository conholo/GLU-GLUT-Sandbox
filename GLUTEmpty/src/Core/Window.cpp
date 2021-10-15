#include "Core/Window.h"

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "freeglut.h"

#include "Core/Input.h"
#include "Core/Display.h"

namespace Core
{
	bool Window::s_Initialized = false;
	unsigned int Window::s_MainWindow = 0;
	unsigned int Window::s_Height = 0;
	unsigned int Window::s_Width = 0;
	Window::WindowCallbackFunction Window::s_EventCallbackFunction;

	void Window::Initialize(const std::string& name, unsigned int width, unsigned int height, const Math::Vector4& clearColor, const WindowCallbackFunction& callback)
	{
		if (s_Initialized) return;
		s_Initialized = true;

		s_Width = width;
		s_Height = height;
		s_EventCallbackFunction = callback;
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

		glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
		glutInitWindowSize(width, height);

		s_MainWindow = glutCreateWindow(name.c_str());
		glutSetWindowTitle(name.c_str());
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

		glutSetWindow(s_MainWindow);
		glutDisplayFunc(Window::OnDisplay);
		glutReshapeFunc(Window::OnResize);
		glutMouseFunc(Window::OnMouseButton);
		glutMouseWheelFunc(Window::OnMouseWheel);
		glutMotionFunc(Window::OnMouseActiveMotion);
		glutKeyboardFunc(Window::OnKeyboardDown);
		glutKeyboardUpFunc(Window::OnKeyboardUp);
		glutPassiveMotionFunc(Window::OnMousePassiveMotion);
		glutIdleFunc(Window::OnIdle);
	}

	void Window::OnResize(int width, int height)
	{
		s_Width = width;
		s_Height = height;

		WindowResizeEvent resizeEvent(width, height);
		s_EventCallbackFunction(resizeEvent);
		glViewport(0, 0, width, height);
	}

	void Window::OnKeyboardDown(unsigned char key, int x, int y)
	{
		KeyPressedEvent keyPressedEvent(key, x, y);
		s_EventCallbackFunction(keyPressedEvent);
	}

	void Window::OnKeyboardUp(unsigned char key, int x, int y)
	{
		KeyReleasedEvent keyReleasedEvent(key, x, y);
		s_EventCallbackFunction(keyReleasedEvent);
	}

	void Window::OnMouseButton(int button, int state, int x, int y)
	{
		MouseButtonEvent mouseButtonEvent(button, state, x, y);
		s_EventCallbackFunction(mouseButtonEvent);
	}

	void Window::OnMouseWheel(int button, int direction, int x, int y)
	{
		MouseWheelEvent mouseWheelEvent(button, direction, x, y);
		s_EventCallbackFunction(mouseWheelEvent);
	}

	void Window::OnMouseActiveMotion(int x, int y)
	{
		MouseActiveMotionEvent mouseActiveMotionEvent(x, y);
		s_EventCallbackFunction(mouseActiveMotionEvent);
	}

	void Window::OnMousePassiveMotion(int x, int y)
	{
		MousePassiveMotionEvent mousePassiveMotionEvent(x, y);
		s_EventCallbackFunction(mousePassiveMotionEvent);
	}

	void Window::OnDisplay()
	{
		DisplayEvent displayEvent;
		s_EventCallbackFunction(displayEvent);
	}

	void Window::OnIdle()
	{
		IdleEvent idleEvent;
		s_EventCallbackFunction(idleEvent);
	}
}