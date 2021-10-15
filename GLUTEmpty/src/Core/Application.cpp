#include "Core/Application.h"
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "Math/Utility.h"
#include "Core/Input.h"
#include "Core/Display.h"
#include "Core/OrbitalControls.h"
#include "Core/Random.h"

namespace Core
{
	Application::Application(const std::string& name)
		:m_Name(name)
	{
		Window::Initialize(name, 800, 800, m_ClearColor, BIND_EVENT_FN(Application::OnEvent));

		Utility::Random::Init();
		m_LayerStack = new LayerStack;
		Display::SetLayerStack(m_LayerStack);
		Random::Init();
	}

	Application::~Application()
	{
		delete m_LayerStack;
		delete m_Window;
	}

	void Application::PushLayer(Layer* layer)
	{
		layer->OnAttach();
		m_LayerStack->PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack->PopLayer(layer);
	}

	void Application::Run()
	{
		glutSetWindow(Window::GetMainWindow());
		glutMainLoop();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonEvent>(BIND_EVENT_FN(Application::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseActiveMotionEvent>(BIND_EVENT_FN(Application::OnMouseActiveMotionEvent));
		dispatcher.Dispatch<MouseWheelEvent>(BIND_EVENT_FN(Application::OnMouseWheelEvent));
		dispatcher.Dispatch<MousePassiveMotionEvent>(BIND_EVENT_FN(Application::OnMousePassiveMotionEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Application::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResizedEvent));
		dispatcher.Dispatch<DisplayEvent>(BIND_EVENT_FN(Application::OnDisplayEvent));
		dispatcher.Dispatch<IdleEvent>(BIND_EVENT_FN(Application::OnIdleEvent));

		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
		{
			if (event.Handled) break;
			(*it)->OnEvent(event);
		}
	}

	bool Application::OnMouseButtonPressed(MouseButtonEvent& mouseButtonEvent)
	{
		Input::OnMouseButton(mouseButtonEvent.GetButton(), mouseButtonEvent.GetState(), mouseButtonEvent.GetMouseX(), mouseButtonEvent.GetMouseY());
		return false;
	}

	bool Application::OnMouseActiveMotionEvent(MouseActiveMotionEvent& mouseMotionEvent)
	{
		Input::OnActiveMouseMotion(mouseMotionEvent.GetMouseX(), mouseMotionEvent.GetMouseY());
		return false;
	}

	bool Application::OnMouseWheelEvent(MouseWheelEvent& mouseWheelEvent)
	{
		if (m_UseOrbitalControls)
			OrbitalControls::OnMouseWheel(mouseWheelEvent.GetDirection());
		return false;
	}

	bool Application::OnMousePassiveMotionEvent(MousePassiveMotionEvent& mouseMotionEvent)
	{
		Input::OnPassiveMouseMotion(mouseMotionEvent.GetMouseX(), mouseMotionEvent.GetMouseY());
		return false;
	}

	bool Application::OnKeyPressedEvent(KeyPressedEvent& keyPressedEvent)
	{
		Input::OnKeyPressed(keyPressedEvent.GetKey());
		return false;
	}

	bool Application::OnKeyReleasedEvent(KeyReleasedEvent& keyReleasedEvent)
	{
		Input::OnKeyReleased(keyReleasedEvent.GetKey());
		return false;
	}

	bool Application::OnWindowResizedEvent(WindowResizeEvent& resizeEvent)
	{
		OrbitalControls::UpdateAspectRatio(resizeEvent.GetWidth(), resizeEvent.GetHeight());
		return false;
	}

	bool Application::OnDisplayEvent(DisplayEvent& displayEvent)
	{
		Display::OnDisplay();
		return false;
	}

	bool Application::OnIdleEvent(IdleEvent& idleEvent)
	{
		Display::OnAnimate();
		return false;
	}
}