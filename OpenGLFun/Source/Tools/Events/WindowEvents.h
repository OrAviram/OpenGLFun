#pragma once
#include "Event.h"

class WindowCloseEvent final : public Event
{
public:
	EventType GetType() const override { return EventType::WindowClose; }
};

class WindowResizeEvent final : public Event
{
public:
	int width, height;

	WindowResizeEvent(int width, int height) : width(width), height(height)
	{
	}

	EventType GetType() const override { return EventType::WindowResize; }
};