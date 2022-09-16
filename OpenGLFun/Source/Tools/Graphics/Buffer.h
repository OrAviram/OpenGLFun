#pragma once
#include "Common.h"

enum class BufferUsage
{
	StaticDraw, DynamicDraw
};

enum class BufferBindingPoint
{
	Array, ElementArray
};

class Buffer
{
	uint id;
	uint size;

public:
	Buffer(uint initialSize, BufferUsage usage = BufferUsage::DynamicDraw);
	Buffer(const void* initialData, uint initialSize, BufferUsage usage = BufferUsage::StaticDraw);
	~Buffer();

	void Bind(BufferBindingPoint bindingPoint) const;
	void SetData(const void* data, BufferUsage usage = BufferUsage::DynamicDraw);
	void SetData(const void* data, uint size, BufferUsage usage = BufferUsage::DynamicDraw);
	void SetSubData(const void* data, uint offset, uint size);
	inline uint GetSize() const { return size; }

	void* GetData();
	inline uint GetID() const { return id; }
};