#pragma once
#include <Tests/Test.h>
#include <Tools/Graphics/Curve.h>
#include <Tools/Graphics/Shader.h>

class Curve3DTest : public Test
{
	Curve3D* curve1;
	Curve3D* curve2;
	Shader* shader;

public:
	Curve3DTest();
	virtual ~Curve3DTest();

	virtual void Update(TimeSpan dt) override;
	virtual void Render(TimeSpan dt) override;
	virtual void RenderGUI(TimeSpan dt) override;
};