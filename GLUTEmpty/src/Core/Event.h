#pragma once

#include "Core/KeyCode.h"
#include "Core/MouseCodes.h"
#include <string>
#include <functional>
#include <iostream>
#include <sstream>


namespace Core
{
#define BIND_EVENT_FN(fn) [this](auto&& ... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

	enum class EventType
	{
		None = 0,
		WindowResize,
		MouseButton, MouseActiveMotion, MousePassiveMotion, MouseWheel,
		KeyPressed, KeyReleased,
		OnDisplay, OnIdle
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::type; }\
								EventType GetEventType() const override { return GetStaticType(); }\
								const char* GetName() const override { return #type; }
	class Event
	{
	public:

		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool Handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			:m_Event(event) { }

		template<typename T, typename Fn>
		bool Dispatch(const Fn& func)
		{
			if (T::GetStaticType() == m_Event.GetEventType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator << (std::ostream& stream, const Event& e)
	{
		return stream << e.ToString();
	}
	
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			:m_Width(width), m_Height(height) { }

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Resized Event: " << "(" << m_Width << "," << m_Height << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
	private:
		int m_Width, m_Height;
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(int button, int state, int x, int y)
			:m_Button((MouseCode)button), m_State(state), m_MouseX(x), m_MouseY(y) { }

		MouseCode GetButton() const { return m_Button; }
		int GetState() const { return m_State; }
		int GetMouseX() const { return m_MouseX; }
		int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Button Event: " << "Button: " << m_Button << ", State: " << m_State;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButton);
	private:
		MouseCode m_Button;
		int m_State;
		int m_MouseX;
		int m_MouseY;
	};

	class MouseWheelEvent : public Event
	{
	public:
		MouseWheelEvent(int button, int direction, int x, int y)
			:m_Button((MouseCode)button), m_Direction(direction), m_MouseX(x), m_MouseY(y) { }

		MouseCode GetButton() const { return m_Button; }
		int GetDirection() const { return m_Direction; }
		int GetMouseX() const { return m_MouseX; }
		int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Wheel Event: " << "Button: " << m_Button << ", Direction: " << m_Direction;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseWheel);
	private:
		MouseCode m_Button;
		int m_Direction;
		int m_MouseX;
		int m_MouseY;
	};

	class MouseActiveMotionEvent : public Event
	{
	public:
		MouseActiveMotionEvent(int x, int y)
			:m_MouseX(x), m_MouseY(y) { }

		int GetMouseX() const { return m_MouseX; }
		int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Active Motion Event: " << "(" << m_MouseX << "," << m_MouseY << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseActiveMotion);
	private:
		int m_MouseX;
		int m_MouseY;
	};

	class MousePassiveMotionEvent : public Event
	{
	public:
		MousePassiveMotionEvent(int x, int y)
			:m_MouseX(x), m_MouseY(y) { }

		int GetMouseX() const { return m_MouseX; }
		int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Passive Motion Event: " << "(" << m_MouseX << "," << m_MouseY << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MousePassiveMotion);
	private:
		int m_MouseX;
		int m_MouseY;
	};

	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(unsigned char key, int x, int y)
			:m_Key(key), m_MouseX(x), m_MouseY(y) { }

		KeyCode GetKey() const { return m_Key; }
		int GetMouseX() const { return m_MouseX; }
		int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Pressed Event: " << m_Key;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	private:
		KeyCode m_Key;
		int m_MouseX;
		int m_MouseY;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(unsigned char key, int x, int y)
			:m_Key(key), m_MouseX(x), m_MouseY(y) { }

		KeyCode GetKey() const { return m_Key; }
		int GetMouseX() const { return m_MouseX; }
		int GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Released Event: " << m_Key;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	private:
		KeyCode m_Key;
		int m_MouseX;
		int m_MouseY;
	};


	class DisplayEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(OnDisplay);
	};

	class IdleEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(OnIdle);
	};
}

