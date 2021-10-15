#pragma once

#include "Core/Layer.h"
#include <vector>

namespace Core
{
	class LayerStack
	{
	public:

		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

	private:
		unsigned int m_LayerInsertIndex = 0;
		std::vector<Layer*> m_Layers;
	};
}