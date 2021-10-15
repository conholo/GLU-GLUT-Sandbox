#include "Core/Display.h"
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#include <iostream>
#include "Core/Window.h"

namespace Core
{
	LayerStack* Display::s_LayerStack = nullptr;
	float Display::s_DeltaTime = 0.0f;
	float Display::s_Elapsed = 0.0f;
	float Display::s_LastFrameTime = 0.0f;

	void Display::OnDisplay()
	{
		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glutSetWindow(Window::GetMainWindow());

		if (s_LayerStack != nullptr)
		{
			for (Layer* layer : *s_LayerStack)
				layer->OnDisplay(s_DeltaTime);
		}

		glutSwapBuffers();
		glFlush();
	}

	void Display::OnAnimate()
	{
		s_Elapsed = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		s_DeltaTime = s_Elapsed - s_LastFrameTime;
		s_LastFrameTime = s_Elapsed;


		if (s_LayerStack != nullptr)
		{
			for (Layer* layer : *s_LayerStack)
				layer->OnAnimate(s_DeltaTime);
		}
	}
}