#include <GL/glew.h>

#include "ShaderCubicCurveTest.h"
#include <Tools/Renderer.h>
#include <Tools/Input.h>
#include <Vendor/ImGUI/imgui.h>

ShaderCubicCurveTest::ShaderCubicCurveTest()
{
	controlPoints = new Point2D[4]
	{
		Point2D(Vector2f(-0.5f, -0.5f)),
		Point2D(Vector2f(-0.25f, 0.5f)),
		Point2D(Vector2f(0.25f, 0.5f)),
		Point2D(Vector2f(0.5f, -0.5f)),
	};
	controlPointsShader = Shader::Load("Resources/Shaders/Basic2DColored.shader");

	const int n = 50;
	float samples[n];
	float d = 1.f / (n - 1);
	float t = 0;
	for (int i = 0; i < n; i++)
	{
		samples[i] = t;
		t += d;
	}

	BufferLayout layout =
	{
		{ ShaderDataType::Float, false }
	};

	RegenerateCPUCurve();
	cpuDividedCurve.va = new VertexArray;
	cpuDividedCurve.va->AddVertexBuffer(cpuDividedCurve.vb, layout);
	cpuDividedCurve.shader = Shader::Load("Resources/Shaders/CubicCurveShaders/CPUDividedCubicCurve.shader");

	float gpuData[] = { 0, 1 };

	Renderer::SetPatchVertexCount(2);
	DivideGPUCurve();
	gpuDividedCurve.shader = Shader::Load("Resources/Shaders/CubicCurveShaders/GPUDividedCubicCurve.shader");
	gpuDividedCurve.vb = new Buffer(gpuData, sizeof(gpuData));
	gpuDividedCurve.va = new VertexArray;
	gpuDividedCurve.va->AddVertexBuffer(gpuDividedCurve.vb, layout);
}

ShaderCubicCurveTest::~ShaderCubicCurveTest()
{
	delete[] controlPoints;
	delete controlPointsShader;

	cpuDividedCurve.DeleteAll();
	gpuDividedCurve.DeleteAll();
}

void ShaderCubicCurveTest::Update(TimeSpan deltaTime)
{
	if (Input::IsKeyDown(KeyCode::A))
		Move(-Vector2f::UnitX * deltaTime * SPEED);

	if (Input::IsKeyDown(KeyCode::W))
		Move(Vector2f::UnitY * deltaTime * SPEED);

	if (Input::IsKeyDown(KeyCode::S))
		Move(-Vector2f::UnitY * deltaTime * SPEED);

	if (Input::IsKeyDown(KeyCode::D))
		Move(Vector2f::UnitX * deltaTime * SPEED);
}

void ShaderCubicCurveTest::Render(TimeSpan deltaTime)
{
	Matrix4x4 transform = Matrix4x4::CreateTranslation(0, 0, zCoordinate);
	transform = Matrix4x4::CreatePerspectiveProjection(Angle::FromDegrees(60), 16 / 9.f, ZMIN, 30) * transform;

	controlPointsShader->Bind();
	controlPointsShader->SetUniformMat4("u_Matrix", transform);

	for (int i = 0; i < 4; i++)
	{
		if ((int)selectedPoint == i)
			controlPointsShader->SetUniformVec4("u_Color", Color::Green);
		else
			controlPointsShader->SetUniformVec4("u_Color", Color::Blue);

		Renderer::DrawPoint(controlPoints[i]);
	}

	Shader* shader = cpuDividedCurve.shader;
	VertexArray* va = cpuDividedCurve.va;
	DrawPrimitive primitive = DrawPrimitive::LineStrips;
	if (divisionType == DivisionType::GPU)
	{
		shader = gpuDividedCurve.shader;
		va = gpuDividedCurve.va;
		primitive = DrawPrimitive::Patches;
	}

	shader->Bind();
	shader->SetUniformMat4("u_Transform", transform);
	shader->SetUniformVec2("u_Control1", controlPoints[0]);
	shader->SetUniformVec2("u_Control2", controlPoints[1]);
	shader->SetUniformVec2("u_Control3", controlPoints[2]);
	shader->SetUniformVec2("u_Control4", controlPoints[3]);
	Renderer::DrawArray(*va, primitive);
}

void ShaderCubicCurveTest::RenderGUI(TimeSpan deltaTime)
{
	ImGui::Begin("Cubic Curve Options");
	{
		ImGui::Combo("Division Type", (int*)(&divisionType), "CPU (vertex shader calculation)\0GPU (tessellation shader calculation)");

		int* divisionPtr = divisionType == DivisionType::CPU ? &cpuDivision : &gpuDivision;
		if (ImGui::DragInt("Division Size", divisionPtr, 1, 1, 64))
		{
			if (divisionType == DivisionType::GPU)
				DivideGPUCurve();
			else
				RegenerateCPUCurve();
		}

		ImGui::DragFloat("Z Coordinate", &zCoordinate, 0.1f, ZMIN, ZMAX);
	}
	ImGui::End();
	// TODO: Add list for changing type and add slider for tessellation levels and reallocate cpu side buffer
}

void ShaderCubicCurveTest::OnKeyPressed(KeyCode key, bool repeat)
{
	if (!repeat)
	{
		if (key == KeyCode::Right)
			AdvanceSelection();
		else if (key == KeyCode::Left)
			ReturnSelection();
	}
}

void ShaderCubicCurveTest::RegenerateCPUCurve()
{
	float* samples = Stackalloc(float, cpuDivision);
	float d = 1.f / (cpuDivision - 1);
	float t = 0;
	for (int i = 0; i < cpuDivision; i++)
	{
		samples[i] = t;
		t += d;
	}

	if (cpuDividedCurve.vb)
		cpuDividedCurve.vb->SetData(samples, cpuDivision * sizeof(float));
	else
		cpuDividedCurve.vb = new Buffer(samples, cpuDivision * sizeof(float));
}

void ShaderCubicCurveTest::DivideGPUCurve()
{
	float levels[] = { 1, (float)gpuDivision };
	Renderer::SetPatchOuterLevels(levels);
}

void ShaderCubicCurveTest::AdvanceSelection()
{
	switch (selectedPoint)
	{
		case ControlPointSelection::A:
			selectedPoint = ControlPointSelection::B;
			break;
		case ControlPointSelection::B:
			selectedPoint = ControlPointSelection::C;
			break;
		case ControlPointSelection::C:
			selectedPoint = ControlPointSelection::D;
			break;
		case ControlPointSelection::D:
			selectedPoint = ControlPointSelection::A;
			break;
	}
}

void ShaderCubicCurveTest::ReturnSelection()
{
	switch (selectedPoint)
	{
		case ControlPointSelection::A:
			selectedPoint = ControlPointSelection::D;
			break;
		case ControlPointSelection::B:
			selectedPoint = ControlPointSelection::A;
			break;
		case ControlPointSelection::C:
			selectedPoint = ControlPointSelection::B;
			break;
		case ControlPointSelection::D:
			selectedPoint = ControlPointSelection::C;
			break;
		default:
			break;
	}
}

void ShaderCubicCurveTest::Move(Vector2f displacement)
{
	Point2D& point = controlPoints[(int)selectedPoint];
	point.SetData(point.Value() + displacement);
}