#include "PolynomialInterpolationTest.h"

#include <Vendor/ImGUI/imgui.h>
#include <Tools/Renderer.h>
#include <Tools/Graphics/Shader.h>
#include <Tools/Input.h>
#include <string>
#include <iostream>

PolynomialInterpolationTest::PolynomialInterpolationTest()
{
	times = { 0, 1, 2, 3 };
	controlPointsEditor.AddPoints(
	{
		{ Vector2f(-200, -100) },
		{ Vector2f(-100, 100) },
		{ Vector2f(100, 100) },
		{ Vector2f(200, -100) },
	});

	controlPointsEditor.onPositionChanged =
		OnPositionChangedFunction::FromMethod<PolynomialInterpolationTest, &PolynomialInterpolationTest::OnControlPointMoved>(this);

	curve = new Curve2D;
	curveShader = Shader::Load("Resources/Shaders/Basic2DColored.shader");
	GenerateCurve();
}

PolynomialInterpolationTest::~PolynomialInterpolationTest()
{
	delete curve;
	delete curveShader;
}

void PolynomialInterpolationTest::Update(TimeSpan dt)
{
	controlPointsEditor.Update(dt);
}

void PolynomialInterpolationTest::Render(TimeSpan dt)
{
	controlPointsEditor.Render();

	curveShader->Bind();
	curveShader->SetUniformMat4("u_Matrix", controlPointsEditor.GetCameraTransform());
	curveShader->SetUniformVec4("u_Color", Color::White);
	Renderer::DrawCurve(*curve);
}

void PolynomialInterpolationTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Curve Settings");
	{
		if (ImGui::DragInt("Samples Per Segment", &samplesPerSegment, 1, 1, 100))
			GenerateCurve();

		controlPointsEditor.DrawGUI();

		if (ImGui::Button("Add Point"))
			AddControlPoint();

		ImGui::SameLine();

		if (ImGui::Button("Remove Point") && controlPointsEditor.PointsCount() > 1)
			RemoveControlPoint();

		ImGui::Separator();
		ImGui::Text("Time Intervals");
		DrawTimeIntervalsGUI();
	}
	ImGui::End();
}

void PolynomialInterpolationTest::DrawTimeIntervalsGUI()
{
	bool regenerate = false;
	for (int i = 1; i < controlPointsEditor.PointsCount(); i++)
	{
		std::string label = "t" + std::to_string(i) + " - t" + std::to_string(i - 1);
		float dt = times[i] - times[i - 1];
		float max = 5;
		if (i < controlPointsEditor.PointsCount() - 1)
			max = times[i + 1] - times[i - 1];

		float dtGui = dt;
		if (ImGui::DragFloat(label.c_str(), &dtGui, 0.01f, 0, max))
		{
			if (dtGui > 0 && dtGui < max)
				dt = dtGui;

			regenerate = true;
		}

		times[i] = times[i - 1] + dt;
	}

	if (regenerate)
		GenerateCurve();
}

void PolynomialInterpolationTest::OnControlPointMoved(int index, const Vector2f& position)
{
	GenerateCurve();
}

void PolynomialInterpolationTest::AddControlPoint()
{
	const Vector2f& lastPoint = controlPointsEditor.GetLastPoint();
	float lastTime = times[times.size() - 1];
	controlPointsEditor.AddPoints({ lastPoint + Vector2f(100, 0) });
	times.push_back(lastTime + 1);
	GenerateCurve();
}

void PolynomialInterpolationTest::RemoveControlPoint()
{
	controlPointsEditor.Delete(1);
	times.erase(times.end() - 1);
	GenerateCurve();
}

void PolynomialInterpolationTest::GenerateCurve()
{
	int totalSamples = samplesPerSegment * (controlPointsEditor.PointsCount() - 1);
	float dt = (times[times.size() - 1] - times[0]) / totalSamples;

	Vector2f* points = Stackalloc(Vector2f, totalSamples + 1);
	float t = 0;
	for (int i = 0; i < totalSamples; i++)
	{
		points[i] = GetSubCurvePoint(t, 0, controlPointsEditor.PointsCount() - 1);
		t += dt;
	}
	points[totalSamples] = controlPointsEditor.GetLastPoint();

	curve->SetData(points, totalSamples + 1);
}

Vector2f PolynomialInterpolationTest::GetSubCurvePoint(float t, int startIndex, int endIndex)
{
	if (endIndex == 0)
		return controlPointsEditor[startIndex];

	float t1 = times[startIndex];
	float t2 = times[startIndex + endIndex];
	Vector2f q1 = GetSubCurvePoint(t, startIndex, endIndex - 1);
	Vector2f q2 = GetSubCurvePoint(t, startIndex + 1, endIndex - 1);
	return (t2 - t) * q1 / (t2 - t1) + (t - t1) * q2 / (t2 - t1);
}