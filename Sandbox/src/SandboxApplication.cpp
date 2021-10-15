#pragma once
#include "Core.h"
#include "EntryPoint.h"

#include "Project2Layer.h"
#include "Project3Layer.h"
#include "TestTextures/TextureTest.h"

class SandboxApplication : public Core::Application
{
public:

	SandboxApplication()
		:Application("GLUT Sandbox")
	{
		//PushLayer(new TextureTestLayer());
		//PushLayer(new Project2Layer());
		PushLayer(new Project3Layer());
	}
	
	~SandboxApplication()
	{

	}
};

Core::Application* Core::CreateApplication()
{
	return new SandboxApplication;
}