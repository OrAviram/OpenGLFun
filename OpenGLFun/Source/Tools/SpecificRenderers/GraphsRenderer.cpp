#include "GraphsRenderer.h"
#include <Tools/Renderer.h>
#include <Tools/Graphics/VertexArray.h>
#include <Tools/Graphics/Shader.h>
#include <iostream>

GraphsRenderer::GraphsRenderer()
{
	transform = Matrix4x4::Identity;

	shader = Shader::Load("Resources/Shaders/Basic2DColored.shader");
	GenerateGrid(GridInfo(-10, 10, -10, 10, 1, 1));
}

GraphsRenderer::~GraphsRenderer()
{
	delete gridVA;
	delete gridVB;
}

void GraphsRenderer::GenerateGrid(const GridInfo& info)
{
	gridInfo = info;

	// Want to plot the points (n * xStep, m * yStep) for integers n, m such that
	// xMin <= n * xStep <= xMax and yMin <= m * yStep <= yMax.
	// So that n goes from ceil(xMin/xStep) to floor(xMax/xStep), and similar for m.

	int nMin = (int)ceil(info.xMin / info.xStep);
	int nMax = (int)floor(info.xMax / info.xStep);

	int mMin = (int)ceil(info.yMin / info.yStep);
	int mMax = (int)floor(info.yMax / info.yStep);

	int xCount = nMax - nMin + 1;
	int yCount = mMax - mMin + 1;

	int vbSize = 2 * (xCount + yCount - 4) * sizeof(Vector2f);

	if (gridVA == nullptr)
	{
		BufferLayout layout =
		{
			{ ShaderDataType::Vec2, false }
		};

		gridVB = new Buffer(vbSize, BufferUsage::DynamicDraw);
		gridVA = new VertexArray;
		gridVA->AddVertexBuffer(gridVB, layout);
	}
	else
		gridVB->SetData(nullptr, vbSize, BufferUsage::DynamicDraw);

	GenerateGridData(nMin * info.xStep, mMin * info.yStep, xCount, yCount);
}


void GraphsRenderer::DrawGrid(const Color& color)
{
	shader->Bind();
	shader->SetUniformMat4("u_Matrix", transform);
	shader->SetUniformVec4("u_Color", color);
	Renderer::DrawArray(*gridVA, DrawPrimitive::Lines);
}

void GraphsRenderer::GenerateGridData(float xMin, float yMin, int xCount, int yCount)
{
	float yMax = yMin + (yCount - 1) * gridInfo.yStep;
	float x = xMin + gridInfo.xStep;
	int offset = 0;
	int indicesOffset = 0;
	for (int i = 1; i < xCount - 1; i++)
	{
		Vector2f value(x, yMin);
		gridVB->SetSubData(&value, offset, sizeof(Vector2f));

		value.y = yMax;
		gridVB->SetSubData(&value, offset + sizeof(Vector2f), sizeof(Vector2f));

		offset += 2 * sizeof(Vector2f);
		x += gridInfo.xStep;
	}

	float y = yMin + gridInfo.yStep;
	for (int i = 1; i < yCount - 1; i++)
	{
		Vector2f value(xMin, y);
		gridVB->SetSubData(&value, offset, sizeof(Vector2f));

		value.x = x;
		gridVB->SetSubData(&value, offset + sizeof(Vector2f), sizeof(Vector2f));

		offset += 2 * sizeof(Vector2f);
		y += gridInfo.yStep;
	}
}