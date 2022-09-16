#pragma once
#include <Tests/Test.h>
#include <Tools/Graphics/VertexArray.h>
#include <Tools/Graphics/Shader.h>

class TessellationTest : public Test
{
	enum class PrimitiveType { Triangle = 0, Isoline = 1, Quad = 2 };
	enum class ProjectionType { Orthographic = 0, Perspective = 1 };

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

	static constexpr float LARGEST_LEVEL = 64;

	float outerLevels[4] = { 1, 1, 1, 1 };
	float innerLevels[2] = { 1, 1 };

	PrimitiveType selectedPrimitive = PrimitiveType::Triangle;
	Renderable triangle;
	Renderable isoline;
	Renderable quad;

	bool syncLevels = false;
	bool animateLevels = false;
	float animationSpeed = 10;
	int animationDirection = 1;
	
	ProjectionType projectionType = ProjectionType::Orthographic;
	bool rotate = false;
	float rotation = 0;

	Matrix4x4 transformation = Matrix4x4::Identity;

public:
	TessellationTest();
	~TessellationTest();

	virtual void Update(TimeSpan dt) override;
	virtual void Render(TimeSpan dt) override;
	virtual void RenderGUI(TimeSpan dt) override;

private:
	void SetPatchVertices();
	void SyncAllLevels(float value);
};