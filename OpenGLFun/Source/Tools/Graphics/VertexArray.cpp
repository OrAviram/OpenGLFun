#include "VertexArray.h"
#include <GL/glew.h>
#include <iostream>

VertexArray::VertexArray() : id(0)
{
	GLCall(glGenVertexArrays(1, &id));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &id));
}

void VertexArray::Bind()
{
	GLCall(glBindVertexArray(id));
}

void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));
}

static GLenum GetGLType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:
	case ShaderDataType::Vec2:
	case ShaderDataType::Vec3:
	case ShaderDataType::Vec4:
	case ShaderDataType::Mat2:
	case ShaderDataType::Mat3:
	case ShaderDataType::Mat4:
		return GL_FLOAT;

	case ShaderDataType::Int: return GL_INT;
	case ShaderDataType::UInt: return GL_UNSIGNED_INT;
	case ShaderDataType::Double: return GL_DOUBLE;
	}
	ASSERT(false);
	return GL_INVALID_ENUM;
}

void VertexArray::AddVertexBuffer(Buffer* buffer, const BufferLayout& bufferLayout)
{
	ASSERT(bufferLayout.GetElementCount() != 0);

	GLCall(glBindVertexArray(id));
	buffer->Bind(BufferBindingPoint::Array);

	for (int i = 0; i < bufferLayout.GetElementCount(); i++)
	{
		GLCall(glEnableVertexAttribArray(i));
		GLboolean normalize = bufferLayout[i].normalize ? GL_TRUE : GL_FALSE;
		GLCall(glVertexAttribPointer(i, bufferLayout[i].GetComponentCount(), GetGLType(bufferLayout[i].type), normalize, bufferLayout.GetStride(), (void*)(bufferLayout[i].offset)));
	}

	vertexBuffers.push_back({ buffer, bufferLayout.GetStride() });
}

void VertexArray::SetIndexBuffer(Buffer* buffer)
{
	GLCall(glBindVertexArray(id));
	buffer->Bind(BufferBindingPoint::ElementArray);
	indexBuffer = buffer;
}

int VertexArray::GetIndexCount() const
{
	return indexBuffer->GetSize() / sizeof(uint);
}

int VertexArray::GetVertexCount() const
{
	int vertexCount = 0;
	for (size_t i = 0; i < vertexBuffers.size(); i++)
		vertexCount += vertexBuffers[i].buffer->GetSize() / vertexBuffers[i].layoutStride;

	return vertexCount;
}