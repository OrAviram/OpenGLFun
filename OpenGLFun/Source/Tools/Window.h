#pragma once
#include "Common.h"
#include "Math/Math.h"
#include "Events/Event.h"

struct WindowInfo
{
	int width, height;
	char* name;
	bool enableVSync = false;
};

class Window
{
	struct WindowState
	{
		WindowInfo info;
		EventHandler eventCallback;
	} currentState;
	struct GLFWwindow* window;

public:
	inline const WindowInfo& GetState() const { return currentState.info; }
	inline float AspectRatio() const { return (float)currentState.info.width / currentState.info.height; }

	Window(const WindowInfo& windowInfo);
	~Window();
	void SetVSync(bool enabled);

	inline void SetEventCallback(const EventHandler& handler) { currentState.eventCallback = handler; }
	inline GLFWwindow* GetNativeWindow() const { return window; }

private:
	static void OnResize(GLFWwindow* window, int width, int height);
	static void OnClose(GLFWwindow* window);
	static void OnKey(GLFWwindow* window, int keyCode, int scancode, int action, int modifiersBitField);
	static void OnMouseButton(GLFWwindow* window, int button, int action, int modifiersBitField);
	static void OnScroll(GLFWwindow* window, double xOffset, double yOffset);

private:
	static bool glfwInitialized;
};