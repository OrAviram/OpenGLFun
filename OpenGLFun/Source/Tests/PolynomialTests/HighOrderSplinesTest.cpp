#include "HighOrderSplinesTest.h"

#include <Vendor/ImGUI/imgui.h>
#include <Tools/Renderer.h>
#include <Tools/Graphics/Shader.h>
#include <Tools/Input.h>
#include <Tools/GuiUtil.h>
#include <string>
#include <iostream>

HighOrderSplinesTest::HighOrderSplinesTest()
{
	std::initializer_list<Vector2f> initialPoints =
	{
		{ Vector2f(-200, 0) },
		{ Vector2f(-100, -200) },
		{ Vector2f(200, -100) },
		{ Vector2f(-100, 100) },
		{ Vector2f(100, 200) },
		{ Vector2f(200, 100) },
	};
	degree = 3;
	knots = { 0, 0, 0, 1, 2, 3, 3, 3 };

	controlPointsEditor.AddPoints(initialPoints);
	controlPointsEditor.onPositionChanged =
		OnPositionChangedFunction::FromMethod<HighOrderSplinesTest, &HighOrderSplinesTest::OnControlPointMoved>(this);

	controlPolygon = new Curve2D(initialPoints);

	curve = new Curve2D;
	curveShader = Shader::Load("Resources/Shaders/Basic2DColored.shader");
	GenerateCurve();
}

HighOrderSplinesTest::~HighOrderSplinesTest()
{
	delete curve;
	delete curveShader;
}

void HighOrderSplinesTest::Update(TimeSpan dt)
{
	if (showControlPoints)
		controlPointsEditor.Update(dt);
}

void HighOrderSplinesTest::Render(TimeSpan dt)
{
	if (showControlPoints)
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

void HighOrderSplinesTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Curve Settings");
	{
		ImGui::Checkbox("Show Control Polygon", &showControlPolygon);
		ImGui::Checkbox("Show Control Points", &showControlPoints);

		if (ImGui::DragInt("Samples Per Segment", &samplesPerSegment, 1, 1, 100))
			GenerateCurve();

		controlPointsEditor.DrawGUI();
		ImGui::Separator();
		DrawDegreeGUI();

		if (ImGui::Button("Add Point"))
			AddControlPoint();

		ImGui::SameLine();

		if (ImGui::Button("Remove Point") && controlPointsEditor.PointsCount() > degree + 1)
			RemoveControlPoint();

		ImGui::Separator();
		DrawKnotsGUI();
	}
	ImGui::End();
}

void HighOrderSplinesTest::DrawDegreeGUI()
{
	ImGui::Text("Degree: %d", degree);

	ImGui::SameLine();

	if (ImGui::Button("+"))
		IncreaseDegree();

	ImGui::SameLine();

	if (ImGui::Button("-"))
		DecreaseDegree();
}

void HighOrderSplinesTest::DrawKnotsGUI()
{
	const float SPEED = 0.01f;
	bool regenerate = false;

	if (degree == 1)
	{
		for (size_t i = 0; i < knots.size(); i++)
		{
			std::string label = "t" + std::to_string(i + 2);

			float min = i == 0 ? -50 : knots[i - 1];
			float max = i == knots.size() - 1 ? knots[i - 1] + 3 : knots[i + 1];
			regenerate |= GuiUtil::ClampedFloatSlider(label.c_str(), knots.data() + i, SPEED, min, max, false, false);
		}
	}
	else
	{
		size_t sectionsStart = degree - 1;
		size_t sectionsEnd = knots.size() - sectionsStart;

		for (size_t i = 0; i < sectionsStart; i++)
		{
			std::string label = "t" + std::to_string(i + 2);

			float min = i == 0 ? -50 : knots[i - 1];
			float max = knots[i + 1];
			regenerate |= GuiUtil::ClampedFloatSlider(label.c_str(), knots.data() + i, SPEED, min, max);
		}

		ImGui::Separator();

		for (size_t i = sectionsStart; i < sectionsEnd; i++)
		{
			std::string label = "t" + std::to_string(i + 2);

			float min = knots[i - 1];
			float max = knots[i + 1];

			// MAYBE TODO: Count multiplicity and based on that choose whether to allow min or max.
			// (We can't have multiplicity >= degree, otherwise the curve won't be continuous)

			bool canBeMax = i != sectionsEnd - 2 && i != sectionsStart;
			bool canBeMin = i != sectionsStart + 1 && i != sectionsEnd - 1;

			regenerate |= GuiUtil::ClampedFloatSlider(label.c_str(), knots.data() + i, SPEED,
				min, max, canBeMin, canBeMax);
		}

		ImGui::Separator();

		for (size_t i = sectionsEnd; i < knots.size(); i++)
		{
			std::string label = "t" + std::to_string(i + 2);

			float min = knots[i - 1];
			float max = i == knots.size() - 1 ? knots[i - 1] + 5 : knots[i + 1];
			regenerate |= GuiUtil::ClampedFloatSlider(label.c_str(), knots.data() + i, SPEED, min, max);
		}
	}

	if (regenerate)
		GenerateCurve();
}

void HighOrderSplinesTest::OnControlPointMoved(int index, const Vector2f& position)
{
	controlPolygon->SetPoint(index, position);
	GenerateCurve();
}

void HighOrderSplinesTest::IncreaseDegree()
{
	if (controlPointsEditor.PointsCount() < degree + 2)
		return;

	float lastKnotDifference = knots[knots.size() - degree] - knots[knots.size() - degree - 1];
	knots.insert(knots.begin(), knots[0]);

	for (uint i = knots.size() - degree; i < knots.size(); i++)
		knots[i] -= lastKnotDifference;

	degree++;
	GenerateCurve();
}

void HighOrderSplinesTest::DecreaseDegree()
{
	if (degree == 1)
		return;

	float addAmount = knots[knots.size() - degree] - knots[knots.size() - degree - 1];
	for (size_t i = knots.size() - degree + 1; i < knots.size(); i++)
		knots[i] += addAmount;

	knots.erase(knots.begin(), knots.begin() + 1);

	degree--;
	GenerateCurve();
}

void HighOrderSplinesTest::GenerateCurve()
{
	// If degree = d and controlPointsEditor.PointsCount() = n, the segments are defined by the following sets of control points:
	// 1...d+1
	// 2...d+2
	// ...
	// n-d...n
	// There are n-d segments, each having d+1 control points.
	// The ith segment is on the knots [t_(d+i), t_(d+i+1)]

	int segmentCount = controlPointsEditor.PointsCount() - degree;
	int totalSamples = segmentCount * samplesPerSegment;

	uint curveSize = totalSamples + 1;
	if (curve->GetPoints().size() < curveSize)
		curve->SetSize(totalSamples + 1);

	int offset = 0;
	for (int i = 0; i < segmentCount; i++)
	{
		if (GenerateSegment(i, offset))
			offset += samplesPerSegment;
		else
			curveSize -= samplesPerSegment;
	}

	if (curve->GetPoints().size() != curveSize)
		curve->SetSize(curveSize);
}

bool HighOrderSplinesTest::GenerateSegment(int index, int offsetInCurve)
{
	// Call the knots t_2, ..., t_(n+d), so that t_j = knots[j - 2].
	// Call the control points c_1, ..., c_n, so that c_j = controlPointsEditor[j - 1].

	// The ith segment is on the interval [t_(d+1+i), t_(d+1+i + 1)]
	// That means it uses the 2d knots t_(i+2), ..., t_(2d+1+i)
	// And the d+1 control points c_(i+1), ..., c_(d+i+1)

	// Thus, the control points are: controlPointsEditor[i], ..., controlPointsEditor[d+i]
	const Vector2f* controlPoints = controlPointsEditor.PositionsArray() + index;

	// And the knots are: knots[i], ..., knots[2d+i-1]
	const float* knots = this->knots.data() + index;

	// We have t_(d+1+i) = knots[d+i-1] and t_(d+1+i + 1) = knots[d+i] (where 'knots' is the old array)
	float t = knots[degree - 1];
	float t2 = knots[degree];
	if (t == t2)
		return false;
	
	float dt = (t2 - t) / samplesPerSegment;
	for (int i = 0; i <= samplesPerSegment; i++)
	{
		Vector2f point = SampleCurve(t, controlPoints, knots, degree);
		curve->SetPoint(offsetInCurve + i, point);
		t += dt;
	}

	return true;
}

Vector2f HighOrderSplinesTest::SampleCurve(float t, const Vector2f* controlPoints, const float* knots, int degree)
{
	if (degree == 0)
		return controlPoints[0];

	int nextDegree = degree - 1;
	int jumpAmount = this->degree - nextDegree;

	Vector2f p1 = SampleCurve(t, controlPoints, knots, nextDegree);
	Vector2f p2 = SampleCurve(t, controlPoints + 1, knots + 1, nextDegree);
	float t1 = knots[nextDegree];
	float t2 = knots[nextDegree + jumpAmount];

	return ((t2 - t) * p1 + (t - t1) * p2) / (t2 - t1);
}

void HighOrderSplinesTest::AddControlPoint()
{
	Vector2f position = controlPointsEditor.GetLastPoint() + Vector2f(100, 100);
	controlPointsEditor.AddPoints({ position });
	controlPolygon->AddPoint(position);

	int knotIndex = knots.size() - degree;
	float knot = knots[knotIndex];
	float increaseAmount = knot - knots[knotIndex - 1];
	knots.insert(knots.begin() + knotIndex, knot);
	
	for (size_t i = knotIndex + 1; i < knots.size(); i++)
		knots[i] += increaseAmount;

	GenerateCurve();
}

void HighOrderSplinesTest::RemoveControlPoint()
{
	float decreaseAmount = knots[knots.size() - degree] - knots[knots.size() - degree - 1];
	for (size_t i = knots.size() - degree; i < knots.size(); i++)
		knots[i] -= decreaseAmount;

	knots.erase(knots.end() - degree);
	controlPointsEditor.Delete(1);
	controlPolygon->RemovePoint(controlPolygon->GetPointsCount() - 1);

	GenerateCurve();
}