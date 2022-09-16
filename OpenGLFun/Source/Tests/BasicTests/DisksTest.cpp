#include "DisksTest.h"

#include <Vendor/ImGUI/imgui.h>
#include <Tools/Renderer.h>
#include <Tools/Graphics/Shader.h>
#include <string>

DisksTest::DisksTest()
{
	disks =
	{
		{ Vector2f(-100, 0), 30, Color::Red },
		{ Vector2f(50, 50), 10, Color::Blue },
	};

	BufferLayout layout =
	{
		{ ShaderDataType::Vec2, false },
		{ ShaderDataType::Float, false },
		{ ShaderDataType::Vec4, false },
	};

	shader = Shader::Load("Resources/Shaders/Disk.shader");

	disksVB = new Buffer(disks.data(), disks.size() * sizeof(Disk), BufferUsage::DynamicDraw);
	vertexArray = new VertexArray();
	vertexArray->AddVertexBuffer(disksVB, layout);
}

DisksTest::~DisksTest()
{
	delete disksVB;
	delete vertexArray;
	delete shader;
}

void DisksTest::Render(TimeSpan dt)
{
	const WindowInfo& windowInfo = Renderer::GetWindow()->GetState();
	Matrix4x4 projection = Matrix4x4::CreateOrthographicProjection((float)windowInfo.width, (float)windowInfo.height, -1, 1);

	shader->Bind();
	shader->SetUniformMat4("u_Projection", projection);
	Renderer::DrawArray(*vertexArray, DrawPrimitive::Points);
}

void DisksTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Disks");
	{
		for (size_t i = 0; i < disks.size(); i++)
		{
			if (DrawDiskGUI(disks[i], i))
				disksVB->SetSubData(&disks[i], i * sizeof(Disk), sizeof(Disk));
		}

		if (ImGui::Button("Add Disk"))
			AddDisk();
	}
	ImGui::End();
}

void DisksTest::AddDisk()
{
	disks.push_back(Disk());
	disksVB->SetData(disks.data(), disks.size() * sizeof(Disk));
}

bool DisksTest::DrawDiskGUI(Disk& disk, int id)
{
	bool changed = false;
	std::string idStr = std::to_string(id);

	std::string label = "Position##";
	label += idStr;
	changed |= ImGui::DragFloat2(label.c_str(), disk.center.elements, 5, -500, 500);

	label = "Radius##";
	label += idStr;
	changed |= ImGui::DragFloat(label.c_str(), &disk.radius, 5, 1, 300);

	label = "Color##";
	label += idStr;
	changed |= ImGui::ColorEdit4(label.c_str(), disk.color.channels);

	ImGui::Separator();

	return changed;
}