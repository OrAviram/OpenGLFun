#include "CubicCurveTest.h"
#include "Tools/Renderer.h"
#include "Vendor/ImGUI/imgui.h"
#include <iostream>

CubicCurveTest::CubicCurveTest()
{
	t1 = new Curve2D({
		{ -0.5f, -0.5f },
		{ -0.25f, 0.5f }});

	t2 = new Curve2D({
		{ 0.25f, 0.5f },
		{ 0.5f, -0.5f }});

	pointCount = 10;
	
	curve = new Curve2D;
	curveData = CubicCurve2D::FromBezier(t1->GetPoint(0), t1->GetPoint(1), t2->GetPoint(0), t2->GetPoint(1));
	Regenerate();

	shader = Shader::Load("Resources/Shaders/Basic2DColored.shader");
}

CubicCurveTest::~CubicCurveTest()
{
	delete curve;
	delete shader;
	delete t1;
	delete t2;
}

void CubicCurveTest::Render(TimeSpan dt)
{
	shader->Bind();
	shader->SetUniformMat4("u_Matrix", Matrix4x4::Identity);

	shader->SetUniformVec4("u_Color", curveColor);
	Renderer::DrawCurve(*curve);

	shader->SetUniformVec4("u_Color", tangentColor);
	Renderer::DrawCurve(*t1);
	Renderer::DrawCurve(*t2);
}

void CubicCurveTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Curve Controls");
	bool anyChange = false;

	if (ImGui::CollapsingHeader("Polynomial Coefficients"))
	{
		Vector2f c1 = curveData.coefficient0, c2 = curveData.coefficient1, c3 = curveData.coefficient2, c4 = curveData.coefficient3;
		bool change1 = ImGui::InputFloat2("1", c1.elements, 5);
		bool change2 = ImGui::InputFloat2("t", c2.elements, 5);
		bool change3 = ImGui::InputFloat2("t^2", c3.elements, 5);
		bool change4 = ImGui::InputFloat2("t^3", c4.elements, 5);
		anyChange |= change1 || change2 || change3 || change4;
		if (anyChange)
			curveData = CubicCurve2D(c1, c2, c3, c4);
	}

	if (ImGui::CollapsingHeader("Points and Tangents"))
	{
		Vector2f point1 = curveData.GetPoint0();
		Vector2f tangent1 = curveData.GetTangent0();
		Vector2f point2 = curveData.GetPoint1();
		Vector2f tangent2 = curveData.GetTangent1();

		bool change1 = ImGui::SliderFloat2("Point 1", point1.elements, -1, 1);
		bool change2 = ImGui::SliderFloat2("Tangent 1", tangent1.elements, -10, 10);
		bool change3 = ImGui::SliderFloat2("Point2", point2.elements, -1, 1);
		bool change4 = ImGui::SliderFloat2("Tangent 2", tangent2.elements, -10, 10);
		anyChange |= change1 || change2 || change3 || change4;
		if (change1 || change2 || change3 || change4)
			curveData = CubicCurve2D::FromTangents(point1, tangent1, point2, tangent2);
	}

	Vector2f bezierPoints[4];
	bezierPoints[0] = curveData.GetBezierControl1();
	bezierPoints[1] = curveData.GetBezierControl2();
	bezierPoints[2] = curveData.GetBezierControl3();
	bezierPoints[3] = curveData.GetBezierControl4();

	if (ImGui::CollapsingHeader("Bezier Control Points"))
	{

		bool change1 = ImGui::SliderFloat2("P1", (float*)bezierPoints, -1, 1);
		bool change2 = ImGui::SliderFloat2("P2", (float*)(bezierPoints + 1), -1, 1);
		bool change3 = ImGui::SliderFloat2("P3", (float*)(bezierPoints + 2), -1, 1);
		bool change4 = ImGui::SliderFloat2("P4", (float*)(bezierPoints + 3), -1, 1);
		anyChange |= change1 || change2 || change3 || change4;
		if (change1 || change2 || change3 || change4)
			curveData = CubicCurve2D::FromBezier(bezierPoints[0], bezierPoints[1], bezierPoints[2], bezierPoints[3]);
	}

	if (anyChange)
	{
		t1->SetData(bezierPoints, 2);
		t2->SetData(bezierPoints + 2, 2);
	}

	ImGui::Text("Draw Settings");
	ImGui::SliderInt("Curve Point Count", &pointCount, 2, 100);
	ImGui::ColorEdit4("Curve Color", (float*)&curveColor);
	ImGui::ColorEdit4("Tangents Color", (float*)&tangentColor);

	if (ImGui::Button("Generate"))
		Regenerate();

	ImGui::End();
}

void CubicCurveTest::Regenerate()
{
	Vector2f* points = Stackalloc(Vector2f, pointCount);
	float d = 1.f / (pointCount - 1);
	float a = 0;
	for (int i = 0; i < pointCount; i++)
	{
		points[i] = curveData.GetPoint(a); //bSqr * b * p1 + 3 * bSqr * a * p2 + 3 * b * aSqr * p3 + aSqr * a * p4;
		a += d;
	}
	curve->SetData(points, pointCount);
}