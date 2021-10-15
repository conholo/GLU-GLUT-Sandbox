#pragma once

#include <glm/glm.hpp>

namespace Core
{
	class Random
	{
	public:
		static void Init();
		static float RandomRange(float min, float max);
	};
}