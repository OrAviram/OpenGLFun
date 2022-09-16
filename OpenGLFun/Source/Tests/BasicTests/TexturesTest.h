#pragma once
#include <Tests/Test.h>
#include <Tools/Graphics/Buffer.h>
#include <Tools/Graphics/VertexArray.h>
#include <Tools/Graphics/Texture.h>
#include <Tools/Graphics/Shader.h>

class TexturesTest : public Test
{
	Shader* shader;
	VertexArray* va;
	Buffer* vb;
	Buffer* ib;
	Matrix4x4 projection;

	Texture* texture1;
	Vector3f translation1 = Vector3f(-300, 0, 0);
	Vector3f scale1 = Vector3f(50, 50, 1);
	Angle rotation1;
	Matrix4x4 model1;

	Texture* texture2;
	Vector3f translation2 = Vector3f(300, 0, 0);
	Vector3f scale2 = Vector3f(50, 50, 1);
	Angle rotation2;
	Matrix4x4 model2;

public:
	TexturesTest();
	~TexturesTest();

	virtual void Update(TimeSpan dt) override;
	virtual void Render(TimeSpan dt) override;
	virtual void RenderGUI(TimeSpan dt) override;
};