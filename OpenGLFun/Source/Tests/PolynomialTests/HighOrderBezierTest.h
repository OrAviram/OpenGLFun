#pragma once
#include <vector>
#include <Tests/Test.h>
#include <Tools/Graphics/Curve.h>
#include <Tests/Utilities/ControlPointsEditor.h>

class HighOrderBezierTest : public Test
{
	ControlPointsEditor controlPointsEditor;
	Curve2D* controlPolygon;
	bool showControlPolygon = true;

	Curve2D* curve;
	Shader* curveShader;
	int samplesPerSegment = 10;

public:
	HighOrderBezierTest();
	~HighOrderBezierTest();

	virtual void Update(TimeSpan deltaTime) override;
	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

private:
	void OnControlPointMoved(int index, const Vector2f& position);

	void GenerateCurve();
	Vector2f GetSubCurvePoint(float t, int startIndex, int endIndex);
	void AddControlPoint();
	void RemoveControlPoint();
};