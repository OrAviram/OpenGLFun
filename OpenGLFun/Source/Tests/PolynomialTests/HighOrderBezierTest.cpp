#include "HighOrderBezierTest.h"

#include <Vendor/ImGUI/imgui.h>
#include <Tools/Renderer.h>
#include <Tools/Graphics/Shader.h>
#include <Tools/Input.h>
#include <string>
#include <iostream>

HighOrderBezierTest::HighOrderBezierTest()
{
	std::initializer_list<Vector2f> initialPoints =
	{
		{ Vector2f(-200, -100) },
		{ Vector2f(-100, 100) },
		{ Vector2f(100, 100) },
		{ Vector2f(200, -100) },
	};

	controlPointsEditor.AddPoints(initialPoints);
	controlPointsEditor.onPositionChanged =
		OnPositionChangedFunction::FromMethod<HighOrderBezierTest, &HighOrderBezierTest::OnControlPointMoved>(this);

	controlPolygon = new Curve2D(initialPoints);

	curve = new Curve2D;
	curveShader = Shader::Load("Resources/Shaders/Basic2DColored.shader");
	GenerateCurve();
}

HighOrderBezierTest::~HighOrderBezierTest()
{
	delete curve;
	delete curveShader;
}

void HighOrderBezierTest::Update(TimeSpan dt)
{
	controlPointsEditor.Update(dt);
}

void HighOrderBezierTest::Render(TimeSpan dt)
{
	controlPointsEditor.Render();

	curveShader->Bind();
	curveShader->SetUniformMat4("u_Matrix", controlPointsEditor.GetCameraTransform());

	if (showControlPolygon)
	{
		curveShader->SetUniformVec4("u_Color", Color::Magenta);
		Renderer::DrawCurve(*controlPolygon);
	}

	curveShader->SetUniformVec4("u_Color", Color::White);
	Renderer::DrawCurve(*curve);
}

void HighOrderBezierTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Curve Settings");
	{
		ImGui::Checkbox("Show Control Polygon", &showControlPolygon);

		if (ImGui::DragInt("Samples Per Segment", &samplesPerSegment, 1, 1, 100))
			GenerateCurve();

		controlPointsEditor.DrawGUI();

		if (ImGui::Button("Add Point"))
			AddControlPoint();

		ImGui::SameLine();

		if (ImGui::Button("Remove Point") && controlPointsEditor.PointsCount() > 1)
			RemoveControlPoint();
	}
	ImGui::End();
}

void HighOrderBezierTest::OnControlPointMoved(int index, const Vector2f& position)
{
	controlPolygon->SetPoint(index, position);
	GenerateCurve();
}

void HighOrderBezierTest::AddControlPoint()
{
	const Vector2f& position = controlPointsEditor.GetLastPoint() + Vector2f(0, -100);
	controlPointsEditor.AddPoints({ position });
	controlPolygon->AddPoint(position);
	GenerateCurve();
}

void HighOrderBezierTest::RemoveControlPoint()
{
	controlPointsEditor.Delete(1);
	controlPolygon->RemovePoint(controlPolygon->GetPointsCount() - 1);
	GenerateCurve();
}

void HighOrderBezierTest::GenerateCurve()
{
	int totalSamples = samplesPerSegment * (controlPointsEditor.PointsCount() - 1);
	float dt = 1.f / totalSamples;

	Vector2f* points = Stackalloc(Vector2f, totalSamples + 1);
	float t = 0;
	for (int i = 0; i < totalSamples; i++)
	{
		points[i] = GetSubCurvePoint(t, controlPointsEditor.PointsCount() - 1, controlPointsEditor.PointsCount() - 1);
		t += dt;
	}
	points[totalSamples] = controlPointsEditor.GetLastPoint();

	curve->SetData(points, totalSamples + 1);
}

Vector2f HighOrderBezierTest::GetSubCurvePoint(float t, int startIndex, int endIndex)
{
	if (endIndex == 0)
		return controlPointsEditor[startIndex];

	Vector2f q1 = GetSubCurvePoint(t, startIndex - 1, endIndex - 1);
	Vector2f q2 = GetSubCurvePoint(t, startIndex, endIndex - 1);
	return (1 - t) * q1 + t * q2;
}