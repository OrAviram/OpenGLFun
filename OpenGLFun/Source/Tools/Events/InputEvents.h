#pragma once
#include "Event.h"
#include "KeyCode.h"
#include "MouseButton.h"

class KeyPressedEvent final : public Event
{
public:
	KeyCode key;
	bool repeat;

	KeyPressedEvent(KeyCode key, bool repeat) : key(key), repeat(repeat)
	{
	}

	EventType GetType() const override { return EventType::KeyPressedEvent; }
};

class KeyReleasedEvent final : public Event
{
public:
	KeyCode key;

	KeyReleasedEvent(KeyCode key) : key(key)
	{
	}

	EventType GetType() const override { return EventType::KeyReleasedEvent; }
};

class MouseButtonPressedEvent final : public Event
{
public:
	MouseButton button;

	MouseButtonPressedEvent(MouseButton button) : button(button)
	{
	}

	EventType GetType() const override { return EventType::MouseButtonPressedEvent; }
};

class MouseButtonReleasedEvent final : public Event
{
public:
	MouseButton button;

	MouseButtonReleasedEvent(MouseButton button) : button(button)
	{
	}

	EventType GetType() const override { return EventType::MouseButtonReleasedEvent; }
};

class ScrollEvent final : public Event
{
public:
	float amount;

	ScrollEvent(float amount) : amount(amount)
	{
	}

	EventType GetType() const override { return EventType::ScrollEvent; }
};