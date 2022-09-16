#pragma once
#include "Common.h"
#include "Math/Vector.h"
#include "Events/KeyCode.h"

class Input
{
	static std::unordered_map<KeyCode, bool> wasJustPressedStatus;

public:
	static void Initialize();
	static void DeInitialize();
	static bool IsKeyDown(KeyCode key);
	//static bool KeyJustPressed(KeyCode key);
	//static bool KeyJustReleased(KeyCode key);

	static Vector2f GetCursorPosition();

private:
	static void OnKeyPressed(KeyCode key, bool repeat);
	static void OnKeyReleased(KeyCode key);
};