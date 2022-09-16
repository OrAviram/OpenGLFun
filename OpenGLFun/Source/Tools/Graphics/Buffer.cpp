#include "Buffer.h"
#include <GL/glew.h>

static GLenum GetGLBindingPoint(BufferBindingPoint bindingPoint)
{
	switch (bindingPoint)
	{
		case BufferBindingPoint::Array: return GL_ARRAY_BUFFER;
		case BufferBindingPoint::ElementArray: return GL_ELEMENT_ARRAY_BUFFER;
	}
	ASSERT(false);
	return -1;
}

static GLenum GetGLUsage(BufferUsage usage)
{
	switch (usage)
	{
		case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
		case BufferUsage::StaticDraw: return GL_STATIC_DRAW;
	}
	ASSERT(false);
	return -1;
}

Buffer::Buffer(const void* initialData, uint size, BufferUsage usage)
{
	this->size = size;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, initialData, GetGLUsage(usage)));
}

Buffer::Buffer(uint size, BufferUsage usage)
{
	this->size = size;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GetGLUsage(usage)));
}

Buffer::~Buffer()
{
	GLCall(glDeleteBuffers(1, &id));
}

void Buffer::Bind(BufferBindingPoint bindingPoint) const
{
	GLCall(glBindBuffer(GetGLBindingPoint(bindingPoint), id));
}

void Buffer::SetData(const void* data, BufferUsage usage)
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GetGLUsage(usage)));
}

void Buffer::SetData(const void* data, uint size, BufferUsage usage)
{
	this->size = size;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GetGLUsage(usage)));
}

void Buffer::SetSubData(const void* data, uint offset, uint size)
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void* Buffer::GetData()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));
	return data;
}