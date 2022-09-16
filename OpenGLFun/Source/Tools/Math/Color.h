#pragma once
#include "Vector.h"

struct Color
{
	union
	{
		struct { float red, green, blue, alpha; };
		float channels[4];
	};

	constexpr Color() : red(0), green(0), blue(0), alpha(1) { }

	// I used to have operations but I'm lazy now. Visual Studio sucks. :(
	constexpr Color(float red, float green, float blue, float alpha) : red(red), green(green), blue(blue), alpha(alpha)
	{
	}

	constexpr Color(const Vector4f& vector) : red(vector.x), green(vector.y), blue(vector.z), alpha(vector.w)
	{
	}

	inline operator Vector4f() const { return Vector4f(red, green, blue, alpha); }

	static const Color Black;

	static const Color Red;
	static const Color Green;
	static const Color Blue;
	
	static const Color Magenta;
	static const Color Yellow;
	static const Color Cyan;

	static const Color White;
	static const Color DarkGray;
	static const Color MiddleGray;
	static const Color LightGray;
};