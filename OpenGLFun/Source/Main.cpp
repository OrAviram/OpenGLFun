#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Tools/Renderer.h"
#include "Tools/Input.h"

#include "Tests/TestSelector.h"
#include "Tests/BasicTests/ClearColorTest.h"
#include "Tests/BasicTests/CompleteMessTest.h"
#include "Tests/BasicTests/TexturesTest.h"
#include "Tests/BasicTests/Curve3DTest.h"
#include "Tests/BasicTests/TessellationTest.h"
#include "Tests/BasicTests/DisksTest.h"

#include "Tests/PolynomialTests/CubicCurveTest.h"
#include "Tests/PolynomialTests/ShaderCubicCurveTest.h"
#include "Tests/PolynomialTests/CubicSplinesTest.h"
#include "Tests/PolynomialTests/PolynomialInterpolationTest.h"
#include "Tests/PolynomialTests/HighOrderBezierTest.h"
#include "Tests/PolynomialTests/HighOrderSplinesTest.h"

#include "Tests/GraphsTest.h"

#include "Tools/SpecificRenderers/PointsBuffer.h"

struct Disk
{
	Vector2f v;
	float r;
	Color c;
};

TestSelector* testSelector;
PointsBuffer* pointsBuffer;
VertexArray* va;
Shader* s;
int id;

Buffer* MAHMANBUFF;
VertexArray* dsa;
Disk* haha;

void OnKeyPressed(KeyCode key, bool repeat)
{
	testSelector->OnKeyPressed(key, repeat);
}

void OnKeyReleased(KeyCode key)
{
	testSelector->OnKeyReleased(key);
}

void OnMouseButtonPressed(MouseButton button)
{
	testSelector->OnMouseButtonPressed(button);
}

void OnMouseButtonReleased(MouseButton button)
{
	testSelector->OnMouseButtonReleased(button);
}

void OnScroll(float amount)
{
	testSelector->OnScroll(amount);
}

void Update(TimeSpan dt)
{
	testSelector->Update(dt);
}

void Render(TimeSpan deltaTime)
{
	testSelector->Render(deltaTime);

	//s->Bind();
	//s->SetUniformMat4("u_Projection", Matrix4x4::Identity);

	//Renderer::DrawArray(*va, DrawPrimitive::Points);
	//Renderer::DrawArray(*dsa, DrawPrimitive::Points);
}

void GuiUpdate(TimeSpan deltaTime)
{
	testSelector->RenderGUI(deltaTime);
}

void main()
{
	WindowInfo windowInfo;
	windowInfo.width = 1920;
	windowInfo.height = 1080;
	windowInfo.name = "Hello World";
	windowInfo.enableVSync = false;
	Renderer::Initialize(windowInfo);

	Renderer::AddKeyPressedFunction(OnKeyPressedFunction::FromFunction<&OnKeyPressed>());
	Renderer::AddKeyReleasedFunction(OnKeyReleasedFunction::FromFunction<&OnKeyReleased>());
	Renderer::AddMouseButtonPressedFunction(OnMouseButtonPressedFunction::FromFunction<&OnMouseButtonPressed>());
	Renderer::AddMouseButtonReleasedFunction(OnMouseButtonPressedFunction::FromFunction<&OnMouseButtonReleased>());
	Renderer::AddOnScrollFunction(OnScrollFunction::FromFunction<&OnScroll>());
	Renderer::SetUpdateFunction(Update);
	Renderer::SetRenderFunction(Render);
	Renderer::SetGUIRenderFunction(GuiUpdate);

	testSelector = new TestSelector();

	testSelector->AddTest<ClearColorTest>("Basic Tests", "Clear Color Test");
	testSelector->AddTest<CompleteMessTest>("Basic Tests", "Complete Mess");
	testSelector->AddTest<TexturesTest>("Basic Tests", "Textures Test");
	testSelector->AddTest<Curve3DTest>("Basic Tests", "Curve3D Test");
	testSelector->AddTest<TessellationTest>("Basic Tests", "Tessellation Test");
	testSelector->AddTest<DisksTest>("Basic Tests", "Disks Test");

	testSelector->AddTest<CubicCurveTest>("Polynomial Tests", "Cubic Curve Test");
	testSelector->AddTest<ShaderCubicCurveTest>("Polynomial Tests", "Shader Cubic Curve Test");
	testSelector->AddTest<CubicSplinesTest>("Polynomial Tests", "Cubic Splines Test");
	testSelector->AddTest<PolynomialInterpolationTest>("Polynomial Tests", "Polynomial Interpolation Test");
	testSelector->AddTest<HighOrderBezierTest>("Polynomial Tests", "High Order Bezier Test");
	testSelector->AddTest<HighOrderSplinesTest>("Polynomial Tests", "High Order Splines Test");

	testSelector->AddTest<GraphsTest>("Other Tests", "Graphs Test");

	//BufferLayout lay =
	//{
	//	{ ShaderDataType::Vec2, false },
	//	{ ShaderDataType::Float, false },
	//	{ ShaderDataType::Vec4, false },
	//};

	//pointsBuffer = new PointsBuffer();
	//pointsBuffer->Add({ Vector2f(-0.1f, 0), 0.02f, Color::White });
	//id = pointsBuffer->Add({ Vector2f(-0.2f, 0.5f), 0.04f, Color::Blue });
	//pointsBuffer->Add({ Vector2f(0.5f, 0), 0.005f, Color::Yellow });

	//pointsBuffer->Set(id, { Vector2f(0.2f, 0.5f), 0.04f, Color::Green });

	//va = new VertexArray;
	//va->AddVertexBuffer(pointsBuffer->GetVertexBuffer(), lay);

	//s = Shader::Load("Shaders/Disk.shader");

	//Disk disks[] =
	//{
		//{ Vector2f(-0.1f, 0), 0.02f, Color::White },
		//{ Vector2f(-0.2f, 0.5f), 0.04f, Color::Blue },
		//{ Vector2f(0.5f, 0), 0.005f, Color::Yellow },
	//};

	//MAHMANBUFF = new Buffer(disks, 3 * sizeof(Disk));
	//dsa = new VertexArray;
	//dsa->AddVertexBuffer(MAHMANBUFF, lay);

	//haha = (Disk*)glMapNamedBuffer(MAHMANBUFF->GetID(), GL_READ_ONLY);
	//glUnmapNamedBuffer(MAHMANBUFF->GetID());
	//haha[1].v.x += 0.5f;

	Renderer::Run();

	delete testSelector;
	Renderer::Deinitialize();
}