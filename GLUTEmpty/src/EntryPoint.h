#pragma once

#include "glut.h"
#include "Core/Application.h"

extern Core::Application* Core::CreateApplication();

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	auto* app = Core::CreateApplication();
	app->Run();
	delete app;
}