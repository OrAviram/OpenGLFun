#pragma once
#include <vector>
#include <Tests/Test.h>
#include <Tests/Utilities/ControlPointsEditor.h>

class CubicSplinesTest : public Test
{
	static const Matrix4x4 UniformBSplineMatrix;
	static const Matrix4x4 PassThroughSplineMatrix;
	static const Matrix4x4 BezierSplineMatrix;

	struct CurveProfile
	{
		Matrix4x4 splineMatrix;
		const char* label;
		int pointsAddedPerSegment;
	} *currentProfile;

	static constexpr int PROFILE_COUNT = 4;
	int selectedProfile = 0;
	CurveProfile curveProfiles[PROFILE_COUNT] = 
	{
		{ BezierSplineMatrix, "Bezier", 3 },
		{ UniformBSplineMatrix, "Uniform B-Spline", 1 },
		{ PassThroughSplineMatrix, "Pass Through Points", 3 },
		{ Matrix4x4::Identity, "Coefficients", 4 }, // Stupid, but just to test
	};

	ControlPointsEditor controlPointsEditor;

	Buffer* splineVB;
	Buffer* splineIB;
	VertexArray* splineVA;
	Shader* splineShader;

	int curveTessellation = 32;

public:
	CubicSplinesTest();
	~CubicSplinesTest();

	virtual void Update(TimeSpan deltaTime) override;
	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

private:
	void AddSegment();
	void RemoveSegment();
	void UpdateBuffersData();
	void OnControlPointPositionChanged(int index, const Vector2f& position);
};