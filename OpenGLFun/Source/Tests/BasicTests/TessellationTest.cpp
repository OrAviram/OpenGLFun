#include <GL/glew.h> 
#include "TessellationTest.h"
#include "Tools/Renderer.h"
#include "Vendor/ImGUI/imgui.h"

TessellationTest::TessellationTest()
{
	BufferLayout layout =
	{
		{ ShaderDataType::Vec2, false },
	};

	Vector2f triangleInfo[] =
	{
		Vector2f(-0.5f, -0.5f),
		Vector2f(0.5f, -0.5f),
		Vector2f(0, 0.366f),
	};

	triangle.shader = Shader::Load("Resources/Shaders/TessellationTests/TriangleTessellation.shader");
	triangle.vb = new Buffer(triangleInfo, sizeof(triangleInfo));

	triangle.va = new VertexArray;
	triangle.va->AddVertexBuffer(triangle.vb, layout);

	Vector2f isolineInfo[] =
	{
		Vector2f(-0.5f, -0.5f),
		Vector2f(0.5f, 0),
	};

	isoline.shader = Shader::Load("Resources/Shaders/TessellationTests/IsolineTessellation.shader");
	isoline.vb = new Buffer(isolineInfo, sizeof(isolineInfo));

	isoline.va = new VertexArray();
	isoline.va->AddVertexBuffer(isoline.vb, layout);

	Vector2f quadInfo[] =
	{
		Vector2f(-0.5f, -0.75f),
		Vector2f(0.5f, -0.5f),
		Vector2f(-0.4f, 0.4f),
		Vector2f(0.35f, 0.3f),
	};

	quad.shader = Shader::Load("Resources/Shaders/TessellationTests/QuadTessellation.shader");
	quad.vb = new Buffer(quadInfo, sizeof(quadInfo));

	quad.va = new VertexArray();
	quad.va->AddVertexBuffer(quad.vb, layout);
}

TessellationTest::~TessellationTest()
{
	triangle.DeleteAll();
	isoline.DeleteAll();
	quad.DeleteAll();
}

void TessellationTest::Update(TimeSpan dt)
{
	if (animateLevels)
	{
		if (outerLevels[0] >= LARGEST_LEVEL)
			animationDirection = -1;
		
		if (outerLevels[0] <= 1)
			animationDirection = 1;

		outerLevels[0] += animationSpeed * animationDirection * dt;
	}

	if (projectionType == ProjectionType::Orthographic)
	{
		WindowInfo windowState = Renderer::GetWindow()->GetState();
		transformation = Matrix4x4::Identity;
		transformation = Matrix4x4::CreateScale(500) * transformation;
		transformation = Matrix4x4::CreateOrthographicProjection(-windowState.width / 2.f, windowState.width / 2.f, -windowState.height / 2.f, windowState.height / 2.f, -1, 1) * transformation;
	}
	else
	{
		if (rotate)
			rotation += dt;
		else
			rotation = 0;

		transformation = Matrix4x4::Identity;
		transformation = Matrix4x4::CreateRotationX(rotation) * transformation;
		transformation = Matrix4x4::CreateTranslation(0, 0, 3) * transformation;
		transformation = Matrix4x4::CreatePerspectiveProjection(Angle::FromDegrees(80), Renderer::GetWindow()->AspectRatio(), 0.1f, 50) * transformation;
	}
}

void TessellationTest::Render(TimeSpan dt)
{
	triangle.shader->Bind();

	Shader* shader = nullptr;
	VertexArray* va = nullptr;

	switch (selectedPrimitive)
	{
		case PrimitiveType::Triangle:
		{
			shader = triangle.shader;
			va = triangle.va;

			shader->Bind();
			shader->SetUniformFloat("u_InnerLevel", innerLevels[0]);
			shader->SetUniformFloatArray("u_OuterLevels", 3, outerLevels);

			//GLCall(glPatchParameteri(GL_PATCH_VERTICES, 3));
		}
		break;

		case PrimitiveType::Isoline:
		{
			shader = isoline.shader;
			va = isoline.va;

			shader->Bind();
			shader->SetUniformFloat("u_LevelX", outerLevels[1]);
			shader->SetUniformFloat("u_LevelY", outerLevels[0]);

			//GLCall(glPatchParameteri(GL_PATCH_VERTICES, 2));
		}
		break;

		case PrimitiveType::Quad:
		{
			shader = quad.shader;
			va = quad.va;

			shader->Bind();
			shader->SetUniformFloatArray("u_InnerLevels", 2, innerLevels);
			shader->SetUniformFloatArray("u_OuterLevels", 4, outerLevels);

			//GLCall(glPatchParameteri(GL_PATCH_VERTICES, 4));
		}
		break;
	}

	if (shader)
		shader->SetUniformMat4("u_Matrix", transformation);

	if (va)
		Renderer::DrawArray(*va, DrawPrimitive::Patches);
}

void TessellationTest::RenderGUI(TimeSpan dt)
{
	ImGui::Begin("Tessellation Test");
	{
		if (ImGui::Combo("Primitive", (int*)(&selectedPrimitive), "Triangle\0Isoline\0Quad"))
			SetPatchVertices();

		ImGui::Checkbox("Sync Levels", &syncLevels);
		if (syncLevels)
		{
			ImGui::Checkbox("Animate Levels", &animateLevels);

			if (animateLevels)
				ImGui::DragFloat("Animation Speed", &animationSpeed, 1, 0, 2000);
			else
				ImGui::DragFloat("Uniform Level", outerLevels, 0.1f, 1, LARGEST_LEVEL);

			SyncAllLevels(outerLevels[0]);
		}
		else
		{
			animateLevels = false;
			switch (selectedPrimitive)
			{
				case PrimitiveType::Triangle:
				{
					ImGui::DragFloat("Inner Level", innerLevels, 0.1f, 1, LARGEST_LEVEL);
					ImGui::DragFloat3("Outer Levels", outerLevels, 0.1f, 1, LARGEST_LEVEL);
				}
				break;

				case PrimitiveType::Isoline:
				{
					ImGui::DragFloat("X Level", &outerLevels[1], 0.1f, 1, LARGEST_LEVEL);
					ImGui::DragFloat("Y Level", &outerLevels[0], 0.1f, 1, LARGEST_LEVEL);
				}
				break;

				case PrimitiveType::Quad:
				{
					ImGui::DragFloat2("Inner Levels", innerLevels, 0.1f, 1, LARGEST_LEVEL);
					ImGui::DragFloat4("Outer Levels", outerLevels, 0.1f, 1, LARGEST_LEVEL);
				}
				break;
			}
		}

		ImGui::Combo("Projection Type", (int*)(&projectionType), "Orthographic\0Perspective");
		if (projectionType == ProjectionType::Perspective)
			ImGui::Checkbox("Rotate", &rotate);
	}
	ImGui::End();
}

void TessellationTest::SetPatchVertices()
{
	switch (selectedPrimitive)
	{
		case PrimitiveType::Triangle:
			Renderer::SetPatchVertexCount(3);
			break;

		case PrimitiveType::Isoline:
			Renderer::SetPatchVertexCount(2);
			break;

		case PrimitiveType::Quad:
			Renderer::SetPatchVertexCount(4);
			break;
	}
}

void TessellationTest::SyncAllLevels(float value)
{
	for (int i = 0; i < 2; i++)
		innerLevels[i] = value;

	for (int i = 0; i < 4; i++)
		outerLevels[i] = innerLevels[0];
}