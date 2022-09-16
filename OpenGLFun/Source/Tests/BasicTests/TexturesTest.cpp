#include "TexturesTest.h"
#include "Tools/Renderer.h"
#include "Tools/GuiUtil.h"
#include "Vendor/ImGUI/ImGui.h"
#include "Vendor/ImGUI/ImGui_internal.h"
#include <math.h>

TexturesTest::TexturesTest()
{
	BufferLayout layout = {
		{ ShaderDataType::Vec3, false }, // Position
		{ ShaderDataType::Vec2, false }, // Texture Coordinates
	};

	float vertices[] = {
		-1, -1, 0, 0, 0,
		-1, 1, 0, 0, 1,
		1, 1, 0, 1, 1,
		1, -1, 0, 1, 0
	};

	int indices[] = {
		0, 1, 2,
		2, 0, 3
	};

	vb = new Buffer(vertices, sizeof(vertices));
	ib = new Buffer(indices, sizeof(indices));
	va = new VertexArray();
	va->AddVertexBuffer(vb, layout);
	va->SetIndexBuffer(ib);

	texture1 = Texture::Load("Resources/Textures/Crate.png", 0);
	texture2 = Texture::Load("Resources/Textures/Pixel Test.png", 1);

	shader = Shader::Load("Resources/Shaders/SimpleTextureTest.shader");
	shader->Bind();
}

TexturesTest::~TexturesTest()
{
	delete vb;
	delete ib;
	delete va;
	delete shader;
	delete texture1;
	delete texture2;
}

void TexturesTest::Update(TimeSpan dt)
{
	float width = (float)Renderer::GetWindow()->GetState().width;
	float height = (float)Renderer::GetWindow()->GetState().height;
	projection = Matrix4x4::CreateOrthographicProjection(-width / 2, width / 2, -height / 2, height / 2, -1, 1);

	model1 = Matrix4x4::CreateTranslation(translation1) * Matrix4x4::CreateRotationZ(rotation1) * Matrix4x4::CreateScale(scale1);
	model2 = Matrix4x4::CreateTranslation(translation2) * Matrix4x4::CreateRotationZ(rotation2) * Matrix4x4::CreateScale(scale2);
}

void TexturesTest::Render(TimeSpan dt)
{
	shader->Bind();
	shader->SetUniformMat4("u_Projection", projection);

	shader->SetUniformInt("u_Texture", 0);
	shader->SetUniformMat4("u_Model", model1);
	Renderer::DrawArrayIndexed(*va);

	shader->SetUniformInt("u_Texture", 1);
	shader->SetUniformMat4("u_Model", model2);
	Renderer::DrawArrayIndexed(*va);
}

void TexturesTest::RenderGUI(TimeSpan dt)
{
	float width = (float)Renderer::GetWindow()->GetState().width;
	float height = (float)Renderer::GetWindow()->GetState().height;
	Vector3f max(width / 2, height / 2, 1);
	Vector3f min = -max;

	ImGui::Begin("Transforms");
	{
		float angle1 = rotation1.Degrees();
		ImGui::Text("Object 1");
		GuiUtil::Vec3Slider("Position##1", translation1, min, max);
		ImGui::SliderFloat3("Scale##1", scale1.elements, 0, 1000);
		ImGui::SliderFloat("Rotation##1", &angle1, -360, 360);
		rotation1.SetDegrees(angle1);

		ImGui::NewLine();

		float angle2 = rotation2.Degrees();
		ImGui::Text("Object 2");
		GuiUtil::Vec3Slider("Position##2", translation2, min, max);
		ImGui::SliderFloat3("Scale##2", scale2.elements, 0, 1000);
		ImGui::SliderFloat("Rotation##2", &angle2, -360, 360);
		rotation2.SetDegrees(angle2);
	}
	ImGui::End();
}