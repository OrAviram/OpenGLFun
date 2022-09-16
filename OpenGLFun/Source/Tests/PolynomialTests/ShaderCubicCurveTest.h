#pragma once
#include <Tests/Test.h>
#include <Tools/Graphics/Point.h>
#include <Tools/Graphics/Shader.h>

class ShaderCubicCurveTest : public Test
{
	struct Renderable
	{
		VertexArray* va;
		Buffer* vb;
		Shader* shader;

		void DeleteAll()
		{
			delete va;
			delete vb;
			delete shader;
		}
	};

	enum class ControlPointSelection { A = 0, B = 1, C = 2, D = 3 };
	enum class DivisionType { CPU = 0, GPU = 1 };

	const float SPEED = 2;
	ControlPointSelection selectedPoint = ControlPointSelection::A;
	Point2D* controlPoints;
	Shader* controlPointsShader;

	Renderable cpuDividedCurve;
	Renderable gpuDividedCurve;

	DivisionType divisionType;
	int cpuDivision = 10;
	int gpuDivision = 10;

	static constexpr float ZMIN = 0.1f;
	static constexpr float ZMAX = 10;
	float zCoordinate = 2;

public:
	ShaderCubicCurveTest();
	~ShaderCubicCurveTest();

	virtual void Update(TimeSpan deltaTime) override;
	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

	virtual void OnKeyPressed(KeyCode key, bool repeat) override;

private:
	void RegenerateCPUCurve();
	void DivideGPUCurve();
	void AdvanceSelection();
	void ReturnSelection();
	void Move(Vector2f displacement);
};