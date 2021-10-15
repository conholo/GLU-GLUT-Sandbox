#pragma once
#include <stdint.h>

namespace Core
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			ButtonLeft = 0,
			ButtonMiddle = 1,
			ButtonRight = 2,
		};
	}
}
