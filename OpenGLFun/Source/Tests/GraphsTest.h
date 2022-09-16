#pragma once

#include "Test.h"

class GraphsTest : public Test
{
private:
	class GraphsRenderer* renderer;

public:
	GraphsTest();
	~GraphsTest();

	virtual void Update(TimeSpan deltaTime) override;
	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

private:
	void OnWindowResized(int width, int height);
};