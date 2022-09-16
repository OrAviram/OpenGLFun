#pragma once
#include <vector>
#include <Tests/Test.h>

class DisksTest : public Test
{
	struct Disk
	{
		Vector2f center = { 0, 0 };
		float radius = 50;
		Color color = Color::Red;
	};

	std::vector<Disk> disks;
	class Buffer* disksVB;
	class VertexArray* vertexArray;
	class Shader* shader;

public:
	DisksTest();
	~DisksTest();

	virtual void Render(TimeSpan deltaTime) override;
	virtual void RenderGUI(TimeSpan deltaTime) override;

private:
	void AddDisk();
	bool DrawDiskGUI(Disk& disk, int id);
};