#pragma once
#include "Buffer.h"
#include "VertexArray.h"
#include "Tools/Math/Math.h"

template<typename TPoint, ShaderDataType shaderType>
class Point
{
	Buffer buffer;
	TPoint value;
	VertexArray vertexArray;

public:
	Point() : buffer(0)
	{
		InitializeVertexArray();
	}

	Point(const TPoint& data) : buffer(&data, sizeof(TPoint)), value(data)
	{
		InitializeVertexArray();
	}

	template<typename... Args>
	Point(const Args&... args) : buffer(sizeof(TPoint)), value(TPoint(args...))
	{
		buffer.SetData(&value);
		InitializeVertexArray();
	}

	inline const TPoint& Value() const { return value; }

	void SetData(const TPoint& data)
	{
		buffer.SetData(&data);
		value = data;
	}

	inline void BindVertexArray() { vertexArray.Bind(); }
	inline void UnbindVertexArray() { vertexArray.Unbind(); }

	inline operator const TPoint&() { return value; }

private:
	void InitializeVertexArray()
	{
		static BufferLayout layout = { { shaderType, false } };
		vertexArray.AddVertexBuffer(&buffer, layout);
	}
};

typedef Point<Vector2f, ShaderDataType::Vec2> Point2D;
typedef Point<Vector3f, ShaderDataType::Vec3> Point3D;