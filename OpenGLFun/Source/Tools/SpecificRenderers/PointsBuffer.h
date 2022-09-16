#pragma once
#include <Tools/Math/Math.h>
#include <Tools/Graphics/Buffer.h>

class PointsBuffer
{
public:
	struct Point
	{
		Vector2f position;
		float radius;
		Color color;
	};

private:
	Buffer buffer;
	//class Buffer indexBuffer;

	int capacity;
	int size;

public:
	PointsBuffer();
	~PointsBuffer();

	int Add(const Point& point);
	void Set(int id, const Point& point);

private:
	void ExpandCapacity();

public:
	inline Buffer* GetVertexBuffer() { return &buffer; }
	//inline const Buffer* GetIndexBuffer() const { return indexBuffer; }

	inline int GetCapacity() const { return capacity; }
	inline int GetSize() const { return size; }
};