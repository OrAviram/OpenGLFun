#pragma once
#include "Common.h"
#include "Test.h"

class TestSelector : public Test
{
	typedef Test*(*CreateTestFunction)();

	struct TestCreator
	{
		std::string name;
		CreateTestFunction create;
	};

	Test* currentTest;
	std::unordered_map<std::string, std::vector<TestCreator>> tests;

public:
	virtual ~TestSelector() override;

	template<typename T>
	inline void AddTest(const char* category, const char* name)
	{
		TestCreator creator;
		creator.name = name;
		creator.create = CreateTest<T>;
		tests[category].push_back(creator);
	}

	virtual void Update(TimeSpan deltaTime) override;
	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

	virtual void OnKeyPressed(KeyCode key, bool repeat) override;
	virtual void OnKeyReleased(KeyCode key) override;
	virtual void OnMouseButtonPressed(MouseButton button) override;
	virtual void OnMouseButtonReleased(MouseButton button) override;

	virtual void OnScroll(float amount) override;

private:
	template <typename T>
	static Test* CreateTest() { return static_cast<Test*>(new T); }
};