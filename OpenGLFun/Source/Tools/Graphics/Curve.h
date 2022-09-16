#pragma once
#include "Buffer.h"
#include "VertexArray.h"
#include "Tools/Math/Math.h"

template<typename TPoint, ShaderDataType shaderType>
class Curve
{
	Buffer buffer;
	std::vector<TPoint> points;
	VertexArray vertexArray;

public:
	BufferUsage bufferUsage = BufferUsage::DynamicDraw;

public:
	Curve()	: buffer(0), points(), vertexArray()
	{
		InitializeVertexArray();
	}

	Curve(const TPoint* points, int count)
		: buffer(points, count * sizeof(TPoint)), points(), vertexArray()
	{
		this->points.resize(count);
		memcpy(this->points.data(), points, count * sizeof(TPoint));
		InitializeVertexArray();
	}

	Curve(const std::initializer_list<TPoint>& points)
		: buffer(points.begin(), points.size() * sizeof(TPoint)), points(points), vertexArray()
	{
		Vector3f f = Vector3f::UnitY;
		InitializeVertexArray();
	}

	~Curve() { }

	inline const std::vector<TPoint>& GetPoints() const { return points; }
	inline const TPoint& GetPoint(int index) const { return points[index]; }
	inline int GetPointsCount() const { return points.size(); }

	void AddPoint(const TPoint& point)
	{
		points.push_back(point);
		buffer.SetData(points.data(), points.size() * sizeof(TPoint), bufferUsage);
	}

	void SetPoint(int pointIndex, const TPoint& position)
	{
		points[pointIndex] = position;
		buffer.SetSubData(&position, pointIndex * sizeof(TPoint), sizeof(TPoint));
	}

	void RemovePoint(int index)
	{
		points.erase(points.begin() + index);
		buffer.SetData(points.data(), points.size() * sizeof(TPoint), bufferUsage);
	}

	void SetData(const TPoint* points, int count)
	{
		this->points.clear();
		this->points.resize(count);
		memcpy(this->points.data(), points, count * sizeof(TPoint));
		buffer.SetData(points, count * sizeof(TPoint), bufferUsage);
	}

	void SetSize(int pointCount)
	{
		points.resize(pointCount);
		buffer.SetData(points.data(), pointCount * sizeof(TPoint), bufferUsage);
	}

	template<typename... Args>
	inline void AddPoint(const Args&... args) { AddPoint(TPoint(args...)); }

	inline void BindVertexArray() { vertexArray.Bind(); }
	inline void UnbindVertexArray() { vertexArray.Unbind(); }

private:
	void InitializeVertexArray()
	{
		static BufferLayout layout = { { shaderType, false } };
		vertexArray.AddVertexBuffer(&buffer, layout);
	}
};

typedef Curve<Vector2f, ShaderDataType::Vec2> Curve2D;
typedef Curve<Vector3f, ShaderDataType::Vec3> Curve3D;