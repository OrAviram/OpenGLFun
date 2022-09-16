#include "Window.h"
#include "Events/WindowEvents.h"
#include "Events/InputEvents.h"
#include <GLFW/glfw3.h>

bool Window::glfwInitialized = false;

Window::Window(const WindowInfo& windowInfo)
{
	if (!glfwInitialized)
	{
		glfwInit();
		glfwInitialized = true;
	}
	
	window = glfwCreateWindow(windowInfo.width, windowInfo.height, windowInfo.name, NULL, NULL);
	currentState.info = windowInfo;

	glfwSetWindowUserPointer(window, &currentState);
	glfwSetWindowSizeCallback(window, OnResize);
	glfwSetWindowCloseCallback(window, OnClose);
	glfwSetKeyCallback(window, OnKey);
	glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwSetScrollCallback(window, OnScroll);

	glfwMakeContextCurrent(window);
	SetVSync(windowInfo.enableVSync);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

void Window::SetVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	currentState.info.enableVSync = enabled;
}

void Window::OnResize(GLFWwindow* window, int width, int height)
{
	WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
	state.info.width = width;
	state.info.height = height;

	WindowResizeEvent e(width, height);
	state.eventCallback(e);
}

void Window::OnClose(GLFWwindow* window)
{
	WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
	WindowCloseEvent e;
	state.eventCallback(e);
}

void Window::OnKey(GLFWwindow* window, int keyCode, int scancode, int action, int modifiersBitField)
{
	WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
	switch (action)
	{
		case GLFW_PRESS:
		{
			KeyPressedEvent e((KeyCode)keyCode, false);
			state.eventCallback(e);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent e((KeyCode)keyCode, true);
			state.eventCallback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent e((KeyCode)keyCode);
			state.eventCallback(e);
			break;
		}
	}
}

void Window::OnMouseButton(GLFWwindow* window, int button, int action, int modifiersBitField)
{
	WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
	switch (action)
	{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent e((MouseButton)button);
			state.eventCallback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent e((MouseButton)button);
			state.eventCallback(e);
			break;
		}
	}
}

void Window::OnScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
	state.eventCallback(ScrollEvent((float)yOffset));
}