#pragma once

#include <stdint.h>

using MouseCode = uint16_t;

namespace Mouse
{
	enum : MouseCode
	{
		ButtonLeft,
		ButtonRight,
		ButtonMiddle
	};
}