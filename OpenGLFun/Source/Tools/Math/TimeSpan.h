#pragma once

struct TimeSpan
{
	float seconds;
	TimeSpan(float seconds) : seconds(seconds) { }
	operator float() { return seconds; }
};