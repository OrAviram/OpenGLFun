#pragma once
#include "Common.h"
#include "Delegate.h"
#include "Window.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Curve.h"
#include "Graphics/Point.h"
#include "Math/Math.h"
#include "Events/WindowEvents.h"
#include "Events/InputEvents.h"

typedef void(*UpdateFunction)(TimeSpan deltaTime);

typedef Delegate<void, int, int> OnWindowResizedFunction;
typedef DelegateChain<void, int, int> OnWindowResizedDelegate;

typedef Delegate<void, KeyCode, bool> OnKeyPressedFunction;
typedef DelegateChain<void, KeyCode, bool> OnKeyPressedDelegate;

typedef Delegate<void, KeyCode> OnKeyReleasedFunction;
typedef DelegateChain<void, KeyCode> OnKeyReleasedDelegate;

typedef Delegate<void, MouseButton> OnMouseButtonPressedFunction;
typedef DelegateChain<void, MouseButton> OnMouseButtonPressedDelegate;

typedef Delegate<void, MouseButton> OnMouseButtonReleasedFunction;
typedef DelegateChain<void, MouseButton> OnMouseButtonReleasedDelegate;

typedef Delegate<void, float> OnScrollFunction;
typedef DelegateChain<void, float> OnScrollDelegate;

enum class DrawPrimitive
{
	Points,
	Lines,
	LineLoops,
	LineStrips,
	Triangles,
	TriangleStrip,
	TriangleFan,
	Patches,
};

class Renderer
{
	static Window* mainWindow;
	static bool active;
	static Color clearColor;
	static TimeSpan timeStarted;

public:
	static void Initialize(const WindowInfo& windowInfo);
	static void Deinitialize();

	static TimeSpan RunningTime();
	inline static const Color& GetClearColor() { return clearColor; }
	static const Color& SetClearColor(float red, float green, float blue, float alpha = 1);
	inline static const Color& SetClearColor(const Color& color) { return SetClearColor(color.red, color.green, color.blue, color.alpha); }

	static void SetPatchVertexCount(int count);
	static void SetPatchInnerLevels(const float* levels);
	static void SetPatchOuterLevels(const float* levels);

	static void Run();
	inline static void SetUpdateFunction(UpdateFunction f) { updateFunction = f; }
	inline static void SetRenderFunction(UpdateFunction f) { renderFunction = f; }
	inline static void SetGUIRenderFunction(UpdateFunction f) { guiRenderFunction = f; }

	inline static void AddResizeFunction(OnWindowResizedFunction f) { onWindowResized += f; }
	inline static void RemoveResizeFunction(OnWindowResizedFunction f) { onWindowResized -= f; }

	inline static void AddKeyPressedFunction(OnKeyPressedFunction f) { onKeyPressed += f; }
	inline static void AddKeyReleasedFunction(OnKeyReleasedFunction f) { onKeyReleased += f; }
	inline static void RemoveKeyPressedFunction(OnKeyPressedFunction f) { onKeyPressed -= f; }
	inline static void RemoveKeyReleasedFunction(OnKeyReleasedFunction f) { onKeyReleased -= f; }

	inline static void AddMouseButtonPressedFunction(OnMouseButtonPressedFunction f) { onMouseButtonPressed += f; }
	inline static void AddMouseButtonReleasedFunction(OnMouseButtonReleasedFunction f) { onMouseButtonReleased += f; }
	inline static void RemoveMouseButtonPressedFunction(OnMouseButtonPressedFunction f) { onMouseButtonPressed -= f; }
	inline static void RemoveMouseButtonReleasedFunction(OnMouseButtonReleasedFunction f) { onMouseButtonReleased -= f; }

	inline static void AddOnScrollFunction(OnScrollFunction f) { onScroll += f; }
	inline static void RemoveOnScrollFunction(OnScrollFunction f) { onScroll -= f; }

	inline static Window* GetWindow() { return mainWindow; }
	inline static bool Active() { return active; }

	static void DrawArrayIndexed(VertexArray& vertexArray, DrawPrimitive primitive = DrawPrimitive::Triangles);
	static void DrawArray(VertexArray& vertexArray, DrawPrimitive primitive = DrawPrimitive::Triangles);

	template<typename TPoints, ShaderDataType pointShaderType>
	static void DrawCurve(Curve<TPoints, pointShaderType>& curve)
	{
		curve.BindVertexArray();
		DrawArraysLineStrip(curve.GetPointsCount());
	}

	template<typename TPoints, ShaderDataType pointShaderType>
	static void DrawPointsOfCurve(Curve<TPoints, pointShaderType>& curve)
	{
		curve.BindVertexArray();
		DrawArraysPoints(curve.GetPointsCount());
	}

	template<typename TPoints, ShaderDataType pointShaderType>
	static void DrawPoint(Point<TPoints, pointShaderType>& point)
	{
		point.BindVertexArray();
		DrawArraysPoints(1);
	}

private:
	static bool running;
	static int patchVertexCount;

	static void OnEvent(const Event& e);
	static void OnWindowResize(const WindowResizeEvent& e);
	static void OnKeyPressed(const KeyPressedEvent& e);
	static void OnKeyReleased(const KeyReleasedEvent& e);
	static void OnMouseButtonPressed(const MouseButtonPressedEvent& e);
	static void OnMouseButtonReleased(const MouseButtonReleasedEvent& e);
	static void OnScroll(const ScrollEvent& e);

	static UpdateFunction updateFunction;
	static UpdateFunction renderFunction;
	static UpdateFunction guiRenderFunction;

	static OnWindowResizedDelegate onWindowResized;
	static OnKeyPressedDelegate onKeyPressed;
	static OnKeyReleasedDelegate onKeyReleased;
	static OnMouseButtonPressedDelegate onMouseButtonPressed;
	static OnMouseButtonReleasedDelegate onMouseButtonReleased;
	static OnScrollDelegate onScroll;

	static void DrawArraysLineStrip(int count);
	static void DrawArraysPoints(int count);
};