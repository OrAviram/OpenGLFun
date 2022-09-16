#pragma once

#include "Common.h"
#include "BufferLayout.h"
#include "Buffer.h"

class VertexArray
{
	struct VertexBuffer
	{
		Buffer* buffer;
		int layoutStride;
	};

	uint id;
	std::vector<VertexBuffer> vertexBuffers;
	Buffer* indexBuffer;

public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();

	void AddVertexBuffer(Buffer* buffer, const BufferLayout& bufferLayout);
	void SetIndexBuffer(Buffer* buffer);

	int GetIndexCount() const;
	int GetVertexCount() const;
};