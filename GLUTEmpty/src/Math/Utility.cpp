#include "Utility.h"

namespace Utility
{
	float Remap(float value, float from1, float to1, float from2, float to2)
	{
		return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
	}
	float Lerp(float v0, float v1, float t)
	{
		return (1 - t) * v0 + t * v1;
	}
	float Clamp(float value, float min, float max)
	{
		if (value > max)
			value = max;
		if (value < min)
			value = min;
		return value;
	}

	float InverseLerp(float min, float max, float value)
	{
		return Clamp((value - min) / (max - min), 0, 1);
	}

	float SmoothStep(float value, float edge0, float edge1)
	{
		float x = InverseLerp(edge0, edge1, value);
		return x * x * (3.0f - 2.0f * x);
	}

	float SmootherStep(float value, float edge0, float edge1)
	{
		float x = InverseLerp(edge0, edge1, value);
		return x * x * x * (x * (x * 6.0 - 15.0f) + 10.0f);
	}

	int Sign(float value)
	{
		return value < 0.0f ? -1 : 1;
	}
}