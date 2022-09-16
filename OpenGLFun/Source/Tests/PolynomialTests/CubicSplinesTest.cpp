#include "CubicSplinesTest.h"

#include <Vendor/ImGUI/imgui.h>
#include <Tools/Renderer.h>
#include <Tools/Graphics/Shader.h>
#include <Tools/Input.h>
#include <string>
#include <iostream>

const Matrix4x4 CubicSplinesTest::UniformBSplineMatrix = Matrix4x4(
	1, -3, 3, -1,
	4, 0, -6, 3,
	1, 3, 3, -3,
	0, 0, 0, 1) / 6;

const Matrix4x4 CubicSplinesTest::PassThroughSplineMatrix = Matrix4x4(
	1, -11/2.f, 9, -9/2.f,
	0, 9, -45/2.f, 27/2.f,
	0, -9/2.f, 18, -27/2.f,
	0, 1, -9/2.f, 9/2.f);

const Matrix4x4 CubicSplinesTest::BezierSplineMatrix = Matrix4x4(
	1, -3, 3, -1,
	0, 3, -6, 3,
	0, 0, 3, -3,
	0, 0, 0, 1);

CubicSplinesTest::CubicSplinesTest()
{
	controlPointsEditor.AddPoints(
	{
		{ Vector2f(-200, -100) },
		{ Vector2f(-100, 100) },
		{ Vector2f(100, 100) },
		{ Vector2f(200, -100) }
	});

	controlPointsEditor.onPositionChanged =
		OnPositionChangedFunction::FromMethod<CubicSplinesTest, &CubicSplinesTest::OnControlPointPositionChanged>(this);

	controlPointsEditor.settings.defaultColor = Color::Green;
	controlPointsEditor.settings.hoveredColor = Color::Yellow;
	controlPointsEditor.settings.movingColor = Color::Magenta;
	controlPointsEditor.ResetPointColors();

	BufferLayout splineLayout =
	{
		{ ShaderDataType::Vec2, false },
	};

	uint indexCount = 4 * (controlPointsEditor.PointsCount() - 3);
	splineIB = new Buffer(indexCount * sizeof(uint), BufferUsage::DynamicDraw);
	splineVB = new Buffer(controlPointsEditor.PointsCount() * sizeof(Vector2f), BufferUsage::DynamicDraw);

	splineVA = new VertexArray();
	splineVA->AddVertexBuffer(splineVB, splineLayout);
	splineVA->SetIndexBuffer(splineIB);

	splineShader = Shader::Load("Resources/Shaders/CubicSpline2D.shader");
	splineShader->Bind();
	splineShader->SetUniformMat4("u_SplineMatrix", curveProfiles[selectedProfile].splineMatrix);

	float levels[] = { 1, (float)curveTessellation };
	Renderer::SetPatchOuterLevels(levels);
	Renderer::SetPatchVertexCount(4);

	UpdateBuffersData();
}

CubicSplinesTest::~CubicSplinesTest()
{
	delete splineVB;
	delete splineIB;
	delete splineVA;
	delete splineShader;
}

void CubicSplinesTest::Update(TimeSpan dt)
{
	controlPointsEditor.Update(dt);
}

void CubicSplinesTest::Render(TimeSpan dt)
{
	controlPointsEditor.Render();

	splineShader->Bind();
	splineShader->SetUniformMat4("u_Projection", controlPointsEditor.GetCameraTransform());
	Renderer::DrawArrayIndexed(*splineVA, DrawPrimitive::Patches);
}

void CubicSplinesTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Spline Settings");
	{
		const char* profileNames[PROFILE_COUNT];
		for (int i = 0; i < PROFILE_COUNT; i++)
			profileNames[i] = curveProfiles[i].label;

		if (ImGui::Combo("Curve Type", &selectedProfile, profileNames, PROFILE_COUNT))
		{
			splineShader->Bind();
			splineShader->SetUniformMat4("u_SplineMatrix", curveProfiles[selectedProfile].splineMatrix);
			UpdateBuffersData();
		}

		if (ImGui::DragInt("Curve Tessellation", &curveTessellation, 1, 1, 64))
		{
			float levels[] = { 1, (float)curveTessellation };
			Renderer::SetPatchOuterLevels(levels);
		}

		controlPointsEditor.DrawGUI();

		if (ImGui::Button("Add Segment"))
			AddSegment();

		ImGui::SameLine();

		if (ImGui::Button("Remove Segment") && controlPointsEditor.PointsCount() - curveProfiles[selectedProfile].pointsAddedPerSegment >= 4)
			RemoveSegment();
	}
	ImGui::End();
}

void CubicSplinesTest::AddSegment()
{
	Vector2f* offsets = Stackalloc(Vector2f, 4);
	offsets[0] = Vector2f(100, 100);
	offsets[1] = Vector2f(200, 100);
	offsets[2] = Vector2f(300, 0);
	offsets[3] = Vector2f(400, 0);

	Vector2f* addedPoints = Stackalloc(Vector2f, curveProfiles[selectedProfile].pointsAddedPerSegment);
	for (int i = 0; i < curveProfiles[selectedProfile].pointsAddedPerSegment; i++)
		addedPoints[i] = controlPointsEditor.GetPoint(controlPointsEditor.PointsCount() - 1) + offsets[i];

	controlPointsEditor.AddPoints(addedPoints, curveProfiles[selectedProfile].pointsAddedPerSegment);
	UpdateBuffersData();
}

void CubicSplinesTest::RemoveSegment()
{
	controlPointsEditor.Delete(curveProfiles[selectedProfile].pointsAddedPerSegment);
	UpdateBuffersData();
}

void CubicSplinesTest::UpdateBuffersData()
{
	uint d = curveProfiles[selectedProfile].pointsAddedPerSegment;
	uint iMax = (controlPointsEditor.PointsCount() - 4) / d;
	uint indexCount = 4 * (iMax + 1);
	uint* indices = Stackalloc(uint, indexCount);
	for (uint i = 0; i <= iMax; i++)
	{
		uint n = d * i;
		uint baseIndex = 4 * i;
		indices[baseIndex] = n;
		indices[baseIndex + 1] = n + 1;
		indices[baseIndex + 2] = n + 2;
		indices[baseIndex + 3] = n + 3;
	}
	splineIB->SetData(indices, indexCount * sizeof(uint), BufferUsage::DynamicDraw);

	uint newPointsCount = d * iMax + 4;
	uint amountRemoved = controlPointsEditor.PointsCount() - newPointsCount;
	if (amountRemoved > 0)
		controlPointsEditor.Delete(amountRemoved);

	splineVB->SetData(controlPointsEditor.PositionsArray(), controlPointsEditor.PointsCount() * sizeof(Vector2f), BufferUsage::DynamicDraw);
}

void CubicSplinesTest::OnControlPointPositionChanged(int index, const Vector2f& position)
{
	splineVB->SetSubData(&position, index * sizeof(Vector2f), sizeof(Vector2f));
}