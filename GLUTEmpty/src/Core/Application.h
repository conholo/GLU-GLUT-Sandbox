#pragma once

#include "Core/Window.h"
#include "Math/Vector.h"
#include "Core/LayerStack.h"
#include "Core/Event.h"

namespace Core
{
	class Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void Run();
		void OnEvent(Event& event);

		void ToggleOrbitalControls(bool enabled) { m_UseOrbitalControls = enabled; }

		Window* GetWindow() const { return m_Window; }

	private:
		bool OnMouseButtonPressed(MouseButtonEvent& mouseButtonEvent);
		bool OnMouseActiveMotionEvent(MouseActiveMotionEvent& mouseMotionEvent);
		bool OnMouseWheelEvent(MouseWheelEvent& mouseWheelEvent);
		bool OnMousePassiveMotionEvent(MousePassiveMotionEvent& mouseMotionEvent);
		bool OnKeyPressedEvent(KeyPressedEvent& keyPressedEvent);
		bool OnKeyReleasedEvent(KeyReleasedEvent& keyReleasedEvent);
		bool OnWindowResizedEvent(WindowResizeEvent& resizeEvent);
		bool OnDisplayEvent(DisplayEvent& resizeEvent);
		bool OnIdleEvent(IdleEvent& resizeEvent);

	private:
		std::string m_Name;
		bool m_UseOrbitalControls = true;
		LayerStack* m_LayerStack;
		Window* m_Window = nullptr;
		Math::Vector4 m_ClearColor{ 0.1f, 0.1f, 0.1f, 0.1f };
	};

	Application* CreateApplication();
}