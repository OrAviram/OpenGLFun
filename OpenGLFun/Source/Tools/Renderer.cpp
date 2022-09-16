#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Vendor/ImGUI/imgui.h"
#include "Vendor/ImGUI/imgui_impl_glfw.h"
#include "Vendor/ImGUI/imgui_impl_opengl3.h"

Window* Renderer::mainWindow = nullptr;
Color Renderer::clearColor = Color(0, 0, 0, 0);
TimeSpan Renderer::timeStarted = 0;

bool Renderer::running = false;
int Renderer::patchVertexCount = -1;

UpdateFunction Renderer::updateFunction = nullptr;
UpdateFunction Renderer::renderFunction = nullptr;
UpdateFunction Renderer::guiRenderFunction = nullptr;

OnWindowResizedDelegate Renderer::onWindowResized;
OnKeyPressedDelegate Renderer::onKeyPressed;
OnKeyReleasedDelegate Renderer::onKeyReleased;
OnMouseButtonPressedDelegate Renderer::onMouseButtonPressed;
OnMouseButtonReleasedDelegate Renderer::onMouseButtonReleased;
OnScrollDelegate Renderer::onScroll;

void Renderer::Initialize(const WindowInfo& windowInfo)
{
	mainWindow = new Window(windowInfo);
	
	glewInit();
	mainWindow->SetEventCallback(OnEvent);

	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(mainWindow->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
}

void Renderer::Deinitialize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete mainWindow;
}

void Renderer::SetPatchVertexCount(int count)
{
	if (patchVertexCount != count)
	{
		GLCall(glPatchParameteri(GL_PATCH_VERTICES, count));
		patchVertexCount = count;
	}
}

void Renderer::SetPatchInnerLevels(const float* levels)
{
	GLCall(glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, levels));
}

void Renderer::SetPatchOuterLevels(const float* levels)
{
	GLCall(glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, levels));
}

void Renderer::Run()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GLCall(glEnable(GL_DEPTH_TEST));

	running = true;
	float lastUpdateTime = 0;
	float lastRenderTime = 0;
	float lastGuiRenderTime = 0;
	timeStarted = (float)glfwGetTime();
	while(running)
	{
		float time = (float)glfwGetTime();
		float dt = time - lastUpdateTime;
		lastUpdateTime = (float)glfwGetTime();
		if (updateFunction)
			updateFunction(dt);

		time = (float)glfwGetTime();
		dt = time - lastRenderTime;
		lastRenderTime = (float)glfwGetTime();

		GLCall(glClearColor(clearColor.red, clearColor.green, clearColor.blue , clearColor.alpha));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		if (renderFunction)
			renderFunction(dt);

		time = (float)glfwGetTime();
		dt = time - lastGuiRenderTime;
		lastGuiRenderTime = (float)glfwGetTime();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (guiRenderFunction)
			guiRenderFunction(dt);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//if (mainWindow->GetState().enableVSync)
			glfwSwapBuffers(mainWindow->GetNativeWindow());
		//else
		//	glFlush();

		glfwPollEvents();
	}
}

void Renderer::OnEvent(const Event& e)
{
	switch (e.GetType())
	{
		case EventType::WindowResize:
			OnWindowResize((const WindowResizeEvent&)e);
			break;
		case EventType::WindowClose:
			running = false;
			break;
		case EventType::KeyPressedEvent:
			OnKeyPressed((const KeyPressedEvent&)e);
			break;
		case EventType::KeyReleasedEvent:
			OnKeyReleased((const KeyReleasedEvent&)e);
			break;
		case EventType::MouseButtonPressedEvent:
			OnMouseButtonPressed((const MouseButtonPressedEvent&)e);
			break;
		case EventType::MouseButtonReleasedEvent:
			OnMouseButtonReleased((const MouseButtonReleasedEvent&)e);
			break;
		case EventType::ScrollEvent:
			OnScroll((const ScrollEvent&)e);
			break;
	}
}

void Renderer::OnWindowResize(const WindowResizeEvent& e)
{
	glViewport(0, 0, e.width, e.height);
	if (onWindowResized)
		onWindowResized(e.width, e.height);
}

void Renderer::OnKeyPressed(const KeyPressedEvent& e)
{
	if (onKeyPressed)
		onKeyPressed(e.key, e.repeat);
}

void Renderer::OnKeyReleased(const KeyReleasedEvent& e)
{
	if (onKeyReleased)
		onKeyReleased(e.key);
}

void Renderer::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
{
	if (onMouseButtonPressed)
		onMouseButtonPressed(e.button);
}

void Renderer::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
{
	if (onMouseButtonReleased)
		onMouseButtonReleased(e.button);
}

void Renderer::OnScroll(const ScrollEvent& e)
{
	if (onScroll)
		onScroll(e.amount);
}

const Color& Renderer::SetClearColor(float red, float green, float blue, float alpha)
{
	clearColor.red = red;
	clearColor.green = green;
	clearColor.blue = blue;
	clearColor.alpha = alpha;
	GLCall(glClearColor(red, green, blue, alpha));
	return clearColor;
}

TimeSpan Renderer::RunningTime()
{
	return (float)glfwGetTime() - timeStarted;
}

inline GLenum GetGDrawMode(DrawPrimitive primitive)
{
	switch (primitive)
	{
		case DrawPrimitive::Points: return GL_POINTS;
		case DrawPrimitive::Lines: return GL_LINES;
		case DrawPrimitive::LineLoops: return GL_LINE_LOOP;
		case DrawPrimitive::LineStrips: return GL_LINE_STRIP;
		case DrawPrimitive::Triangles: return GL_TRIANGLES;
		case DrawPrimitive::TriangleStrip: return GL_TRIANGLE_STRIP;
		case DrawPrimitive::TriangleFan: return GL_TRIANGLE_FAN;
		case DrawPrimitive::Patches: return GL_PATCHES;
	}
	ASSERT(false);
	return -1;
}

void Renderer::DrawArrayIndexed(VertexArray& vertexArray, DrawPrimitive primitive)
{
	vertexArray.Bind();
	GLCall(glDrawElements(GetGDrawMode(primitive), vertexArray.GetIndexCount(), GL_UNSIGNED_INT, (void*)0));
}

void Renderer::DrawArray(VertexArray& vertexArray, DrawPrimitive primitive)
{
	vertexArray.Bind();
	GLCall(glDrawArrays(GetGDrawMode(primitive), 0, vertexArray.GetVertexCount()));
}

void Renderer::DrawArraysLineStrip(int count)
{
	GLCall(glDrawArrays(GL_LINE_STRIP, 0, count));
}

void Renderer::DrawArraysPoints(int count)
{
	GLCall(glDrawArrays(GL_POINTS, 0, count));
}