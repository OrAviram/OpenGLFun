#include "ClearColorTest.h"
#include <Tools/Renderer.h>
#include <Vendor/ImGUI/imgui.h>

ClearColorTest::ClearColorTest()
{
	clearColor = Renderer::GetClearColor();
}

void ClearColorTest::RenderGUI(TimeSpan deltaTime)
{
	ImGui::Begin("Clear Color Test");
	ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
	ImGui::End();
}

void ClearColorTest::Update(TimeSpan deltaTime)
{
	Renderer::SetClearColor(clearColor);
}