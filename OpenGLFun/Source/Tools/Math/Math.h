#pragma once

#include "Vector.h"
#include "Matrix4x4.h"
#include "Color.h"
#include "TimeSpan.h"
#include "Angle.h"

namespace MathUtil
{
	inline float Clamp(float value, float min, float max)
	{
		if (value <= min)
			return min;

		if (value >= max)
			return max;

		return value;
	}
}