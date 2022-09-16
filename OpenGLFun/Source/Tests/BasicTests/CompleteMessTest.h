#pragma once
#include <Tests/Test.h>
#include <Tools/Graphics/VertexArray.h>
#include <Tools/Graphics/Shader.h>
#include <Tools/Graphics/Curve.h>
#include <Tools/Graphics/Texture.h>
#include <Tools/Renderer.h>

namespace CompleteMessConstants
{
	const int SMOOTHS_AMOUNT = 10;
	const float PI = 3.14159265359f;
}

class CompleteMessTest : public Test
{
	VertexArray* squareVA;
	VertexArray* triangleVA;
	Shader* shader;
	Matrix4x4 projection;
	Matrix4x4 translation;
	Matrix4x4 scale;

	Curve2D* curve;
	Curve2D* smooths[CompleteMessConstants::SMOOTHS_AMOUNT];

	Curve2D* testier;
	Shader* basic;
	Vector2f originScaleTest = Vector2f(400, 100);

	VertexArray* cube;
	Texture* texture;

	Buffer* squareVB;
	Buffer* squareIB;

	Buffer* triangleVB;
	Buffer* triangleIB;

	Buffer* cubeVB;
	Buffer* cubeIB;

	float c = 0;
	float v = 1;
	float dc = v;

	float p = 0;
	float s = 1;
	float dp = s;

	float angle = 0;
	float rotationRate = 0.5f;

	float elapsed = 0;

	Vector2i moveDir;
	Vector3f pos = Vector3f::Zero;

public:
	CompleteMessTest();
	virtual ~CompleteMessTest() override;

	virtual void Render(TimeSpan deltaTime) override;
	virtual void OnKeyPressed(KeyCode key, bool repeat) override;
	virtual void OnKeyReleased(KeyCode key) override;
};