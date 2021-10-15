#pragma once

#include "Core/LayerStack.h"

namespace Core
{
	class Display
	{
	public:
		static void OnDisplay();
		static void OnAnimate();
		static void SetLayerStack(LayerStack* layerStack) { s_LayerStack = layerStack; }
		static float GetDeltaTime() { return s_DeltaTime; }

	private:
		static float s_DeltaTime;
		static float s_Elapsed;
		static float s_LastFrameTime;
		static LayerStack* s_LayerStack;
	};
}