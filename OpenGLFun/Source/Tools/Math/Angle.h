#pragma once

struct Angle
{
	float radians;

	Angle() : radians(0) { }
	Angle(float radians) : radians(radians) { }

	inline operator float() const { return radians; }

	inline float Radians() const { return radians; }
	inline float Degrees() const { return radians * 57.2957795131f; }
	inline float SetDegrees(float value) { return radians = value * 0.01745329251f; }

	inline static Angle FromDegrees(float degrees) { return degrees * 0.01745329251f; }
};