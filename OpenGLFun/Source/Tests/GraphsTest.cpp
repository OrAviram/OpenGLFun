#include "GraphsTest.h"
#include <Tools/Renderer.h>
#include <Tools/SpecificRenderers/GraphsRenderer.h>

GraphsTest::GraphsTest()
{
	renderer = new GraphsRenderer();

	/*auto gridInfo = renderer->GetGridInfo();
	gridInfo.xMin = -5;
	gridInfo.xStep = 0.3f;
	renderer->GenerateGrid(gridInfo);*/

	Renderer::AddResizeFunction(OnWindowResizedFunction::FromMethod<GraphsTest, &GraphsTest::OnWindowResized>(this));

	const auto& windowState = Renderer::GetWindow()->GetState();
	OnWindowResized(windowState.width, windowState.height);
}

GraphsTest::~GraphsTest()
{
	delete renderer;
}

void GraphsTest::Update(TimeSpan deltaTime)
{
}

void GraphsTest::Render(TimeSpan deltaTime)
{
	renderer->DrawGrid(Color::DarkGray);
}

void GraphsTest::RenderGUI(TimeSpan deltaTime)
{

}

void GraphsTest::OnWindowResized(int width, int height)
{
	float xMax = width / 2.f;
	float yMax = height / 2.f;
	renderer->transform = Matrix4x4::CreateOrthographicProjection(-xMax, xMax, -yMax, yMax, -10, 10);

	auto gridInfo = renderer->GetGridInfo();
	gridInfo.xMin = -xMax;
	gridInfo.xMax = xMax;
	gridInfo.yMin = -yMax;
	gridInfo.yMax = yMax;

	gridInfo.xStep = xMax / 10;
	gridInfo.yStep = gridInfo.xStep;
	renderer->GenerateGrid(gridInfo);
}