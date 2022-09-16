#include "Input.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>

std::unordered_map<KeyCode, bool> Input::wasJustPressedStatus;

void Input::Initialize()
{
	Renderer::AddKeyPressedFunction(OnKeyPressedFunction::FromFunction<&OnKeyPressed>());
	Renderer::AddKeyReleasedFunction(OnKeyReleasedFunction::FromFunction<&OnKeyReleased>());
}

void Input::DeInitialize()
{
	Renderer::RemoveKeyPressedFunction(OnKeyPressedFunction::FromFunction<&OnKeyPressed>());
	Renderer::RemoveKeyReleasedFunction(OnKeyReleasedFunction::FromFunction<&OnKeyReleased>());
}

bool Input::IsKeyDown(KeyCode key)
{
	return glfwGetKey(Renderer::GetWindow()->GetNativeWindow(), (int)key);
}

//bool Input::KeyJustPressed(KeyCode key)
//{
//	if (wasJustPressedStatus[key])
//	{
//		wasJustPressedStatus[key] = false;
//		return true;
//	}
//	return false;
//}
//
//bool Input::KeyJustReleased(KeyCode key)
//{
//	return true;
//}

Vector2f Input::GetCursorPosition()
{
	double x, y;
	glfwGetCursorPos(Renderer::GetWindow()->GetNativeWindow(), &x, &y);
	return Vector2f((float)x, (float)y);
}

void Input::OnKeyPressed(KeyCode key, bool repeat)
{
	wasJustPressedStatus[key] = true;
}

void Input::OnKeyReleased(KeyCode key)
{

}