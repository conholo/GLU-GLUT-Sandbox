#pragma once
#include <ctime>
#include <stdlib.h>

namespace Utility
{
	static class Random
	{
	public:
		static void Init()
		{
			srand(static_cast <unsigned> (time(0)));
		}

		static float RandomRange(float min, float max)
		{
			return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		}
	};

	float Remap(float value, float from1, float to1, float from2, float to2);
	float Lerp(float v0, float v1, float t);
	float Clamp(float value, float min, float max);
	float SmoothStep(float value, float edge0, float edge1);
	float SmootherStep(float value, float edge0, float edge1);
	float InverseLerp(float min, float max, float t);
	int Sign(float value);
}