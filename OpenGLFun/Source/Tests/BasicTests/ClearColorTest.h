#pragma once
#include <Tests/Test.h>

class ClearColorTest : public Test
{
	Color clearColor;
public:
	ClearColorTest();

	virtual void Update(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;
};