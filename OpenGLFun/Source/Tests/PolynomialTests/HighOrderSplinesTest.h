#pragma once
#include <vector>
#include <Tests/Test.h>
#include <Tools/Graphics/Curve.h>
#include <Tests/Utilities/ControlPointsEditor.h>

class HighOrderSplinesTest : public Test
{
	ControlPointsEditor controlPointsEditor;
	Curve2D* controlPolygon;
	bool showControlPolygon = true;
	bool showControlPoints = true;

	Curve2D* curve;
	Shader* curveShader;
	int samplesPerSegment = 10;

	int degree;
	std::vector<float> knots;

public:
	HighOrderSplinesTest();
	~HighOrderSplinesTest();

	virtual void Update(TimeSpan deltaTime) override;
	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

private:
	void DrawDegreeGUI();
	void DrawKnotsGUI();

	void OnControlPointMoved(int index, const Vector2f& position);
	void IncreaseDegree();
	void DecreaseDegree();

	void GenerateCurve();
	bool GenerateSegment(int index, int offsetInCurve);
	Vector2f SampleCurve(float t, const Vector2f* controlPoints, const float* knots, int degree);

	void AddControlPoint();
	void RemoveControlPoint();
};