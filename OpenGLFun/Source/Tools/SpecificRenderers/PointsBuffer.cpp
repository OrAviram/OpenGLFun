#include "PointsBuffer.h"
#include <Tools/Graphics/Buffer.h>

PointsBuffer::PointsBuffer()
	: buffer(2 * sizeof(Point), BufferUsage::DynamicDraw),/*, indexBuffer(2 * sizeof(Point), BufferUsage::DynamicDraw)*/
	capacity(2), size(0)
{
}

PointsBuffer::~PointsBuffer()
{
}

int PointsBuffer::Add(const Point& point)
{
	if (size == capacity)
		ExpandCapacity();

	buffer.SetSubData(&point, size * sizeof(Point), sizeof(Point));
	return size++;
}

void PointsBuffer::Set(int id, const Point& point)
{
	buffer.SetSubData(&point, id * sizeof(Point), sizeof(Point));
}

void PointsBuffer::ExpandCapacity()
{
	int newCapacity = capacity * 2;

	void* data = buffer.GetData();
	buffer.SetData(nullptr, newCapacity * sizeof(Point));
	buffer.SetSubData(data, 0, capacity * sizeof(Point));

	//data = indexBuffer.GetData();
	//indexBuffer.SetData(nullptr, capacity * sizeof(uint));
	//indexBuffer.SetSubData(data, 0, capacity * sizeof(uint));

	capacity = newCapacity;
}