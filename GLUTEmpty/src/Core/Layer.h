#pragma once

#include "Core/Event.h"

#include <string>


namespace Core
{
	class Layer
	{
	public:
		Layer(const std::string& layerName = "Layer");
		virtual ~Layer() = default;

		const std::string& GetName() const { return m_LayerName; }

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnDisplay(float dt) { }
		virtual void OnAnimate(float dt) { }
		virtual void OnEvent(Event& event) { }

	private:
		std::string m_LayerName;
	};
}