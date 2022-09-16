#include "Curve3DTest.h"
#include "Vendor/ImGui/ImGui.h"
#include "Tools/Renderer.h"
#include <string>

Curve3DTest::Curve3DTest()
{
	shader = Shader::Load("Resources/Shaders/Basic.shader");
	
	curve1 = new Curve3D(
	{
		{ -5, 3, 5, },
		{ -3, -3, 10 },
		{ 0, -3, 5 },
	});

	curve2 = new Curve3D(
	{
		{ 5, 3, 5, },
		{ 3, -3, 10  },
		{ 2, -3, 5 },
	});
}

Curve3DTest::~Curve3DTest()
{
	delete shader;
	delete curve1;
	delete curve2;
}

void Curve3DTest::Update(TimeSpan dt)
{

}

void Curve3DTest::Render(TimeSpan dt)
{
	shader->Bind();
	shader->SetUniformMat4("u_Matrix", Matrix4x4::CreatePerspectiveProjection(Angle::FromDegrees(120), Renderer::GetWindow()->AspectRatio(), 0.1f, 10));

	Renderer::DrawCurve(*curve1);
	Renderer::DrawCurve(*curve2);
}

static void DrawPointsUI(Curve3D& curve, int id)
{
	for (int i = 0; i < curve.GetPointsCount(); i++)
	{
		std::string str = "Point ";
		str += std::to_string(i);
		str += "##";
		str += id;

		Vector3f point = curve.GetPoint(i);
		if (ImGui::SliderFloat3(str.c_str(), point.elements, -5, 5))
			curve.SetPoint(i, point);

		ImGui::SameLine();
		if (ImGui::Button((std::string("X##") + std::to_string(i) + std::string("##") + std::to_string(id)).c_str()))
			curve.RemovePoint(i);
	}

	if (ImGui::Button((std::string("Add Point##") + std::to_string(id)).c_str()))
		curve.AddPoint(0.f, 0.f, -1.f);
}

void Curve3DTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Curves");

	ImGui::Text("Curve 1");
	DrawPointsUI(*curve1, 1);

	ImGui::Text("Curve 2");
	DrawPointsUI(*curve2, 2);
	
	ImGui::End();
}