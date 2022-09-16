#pragma once
#include "Tools/Math/Math.h"
#include "Tools/Events/KeyCode.h"
#include "Tools/Events/MouseButton.h"

class Test
{
public:
	Test() { }
	virtual ~Test() { }

	virtual void Update(TimeSpan deltaTime) { }
	virtual void Render(TimeSpan deltaTime) { }
	virtual void RenderGUI(TimeSpan deltaTime) { }

	virtual void OnKeyPressed(KeyCode key, bool repeat) { }
	virtual void OnKeyReleased(KeyCode key) { }

	virtual void OnMouseButtonPressed(MouseButton button) { }
	virtual void OnMouseButtonReleased(MouseButton button) { }

	virtual void OnScroll(float amount) { }
};