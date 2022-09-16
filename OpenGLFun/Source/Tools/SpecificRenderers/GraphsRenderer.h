#pragma once
#include <Tools/Math/Math.h>

struct GridInfo
{
	float xStep, yStep;
	float xMin, xMax;
	float yMin, yMax;

	GridInfo() : GridInfo(-10, 10, -10, 10, 1, 1)
	{
	}

	GridInfo(float xMin, float xMax, float yMin, float yMax, float xStep = 1, float yStep = 1)
		: xStep(xStep), yStep(yStep), xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax)
	{
	}
};

class GraphsRenderer
{
public:
	Matrix4x4 transform;

private:
	GridInfo gridInfo;
	class Buffer* gridVB;
	class VertexArray* gridVA;

	class Shader* shader;

public:
	GraphsRenderer();
	~GraphsRenderer();

	void GenerateGrid(const GridInfo& info);
	inline const GridInfo& GetGridInfo() const { return gridInfo; }
	void DrawGrid(const Color& color = Color::DarkGray);

private:
	void GenerateGridData(float xMin, float yMin, int xCount, int yCount);
};