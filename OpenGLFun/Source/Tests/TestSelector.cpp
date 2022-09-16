#include "TestSelector.h"
#include "Vendor/ImGUI/ImGui.h"
#include <string>

TestSelector::~TestSelector()
{
	if (currentTest)
		delete currentTest;
}

void TestSelector::Update(TimeSpan deltaTime)
{
	if (currentTest)
		currentTest->Update(deltaTime);
}

void TestSelector::Render(TimeSpan deltaTime)
{
	if (currentTest)
		currentTest->Render(deltaTime);
}

void TestSelector::RenderGUI(TimeSpan deltaTime)
{
	if (currentTest)
		currentTest->RenderGUI(deltaTime);

	ImGui::Begin("Test Selector");
	if (currentTest)
	{
		if (ImGui::Button("Exit Test"))
		{
			delete currentTest;
			currentTest = nullptr;
		}
	}
	else
	{
		for (auto& [category, creators] : tests)
		{
			std::string label = category + " (" + std::to_string(creators.size()) + ")";
			if (ImGui::CollapsingHeader(label.c_str()))
			{
				for (TestCreator& creator : creators)
				{
					if (ImGui::Button(creator.name.c_str()))
						currentTest = creator.create();
				}
			}
		}
	}
	ImGui::Text(std::to_string(1 / deltaTime).c_str());
	ImGui::SameLine();
	ImGui::Text(" FPS");
	ImGui::End();
}

void TestSelector::OnKeyPressed(KeyCode key, bool repeat)
{
	if (currentTest)
		currentTest->OnKeyPressed(key, repeat);
}

void TestSelector::OnKeyReleased(KeyCode key)
{
	if (currentTest)
		currentTest->OnKeyReleased(key);
}

void TestSelector::OnMouseButtonPressed(MouseButton button)
{
	if (currentTest)
		currentTest->OnMouseButtonPressed(button);
}

void TestSelector::OnMouseButtonReleased(MouseButton button)
{
	if (currentTest)
		currentTest->OnMouseButtonReleased(button);
}

void TestSelector::OnScroll(float amount)
{
	if (currentTest)
		currentTest->OnScroll(amount);
}