#pragma once

enum class EventType
{
	WindowClose, WindowResize,
	KeyPressedEvent, KeyReleasedEvent,
	MouseButtonPressedEvent, MouseButtonReleasedEvent,
	ScrollEvent,
};

class Event
{
public:
	virtual EventType GetType() const = 0;
};

typedef void(*EventHandler)(const Event& e);