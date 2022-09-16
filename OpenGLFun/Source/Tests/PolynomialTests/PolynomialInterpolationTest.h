#pragma once
#include <vector>
#include <Tests/Test.h>
#include <Tools/Graphics/Curve.h>
#include <Tests/Utilities/ControlPointsEditor.h>

class PolynomialInterpolationTest : public Test
{
	ControlPointsEditor controlPointsEditor;
	std::vector<float> times;

	Curve2D* curve;
	Shader* curveShader;
	int samplesPerSegment = 10;

public:
	PolynomialInterpolationTest();
	~PolynomialInterpolationTest();

	virtual void Update(TimeSpan deltaTime) override;
	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

private:
	void DrawTimeIntervalsGUI();

	void GenerateCurve();
	Vector2f GetSubCurvePoint(float t, int startIndex, int endIndex);

	void AddControlPoint();
	void RemoveControlPoint();
	void OnControlPointMoved(int index, const Vector2f& position);
};