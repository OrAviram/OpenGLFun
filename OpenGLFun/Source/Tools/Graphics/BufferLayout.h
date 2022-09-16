#pragma once
#include "Common.h"
#include "ShaderDataType.h"

struct BufferLayoutElement
{
	ShaderDataType type;
	int offset;
	bool normalize;

	BufferLayoutElement(ShaderDataType type, bool normalize) : type(type), offset(0), normalize(normalize) { }
	int GetSize() const
	{
		switch (type)
		{
		case ShaderDataType::Int: return 4;
		case ShaderDataType::UInt: return 4;
		case ShaderDataType::Float: return sizeof(float);
		case ShaderDataType::Double: return sizeof(double);

		case ShaderDataType::Vec2: return 2 * sizeof(float);
		case ShaderDataType::Vec3: return 3 * sizeof(float);
		case ShaderDataType::Vec4: return 4 * sizeof(float);

		case ShaderDataType::Mat2: return 4 * sizeof(float);
		case ShaderDataType::Mat3: return 9 * sizeof(float);
		case ShaderDataType::Mat4: return 16 * sizeof(float);
		}
		ASSERT(false);
		return -1;
	}

	int GetComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::Int:
		case ShaderDataType::UInt:
		case ShaderDataType::Float:
		case ShaderDataType::Double:
			return 1;

		case ShaderDataType::Vec2: return 2;
		case ShaderDataType::Vec3: return 3;
		case ShaderDataType::Vec4: return 4;

		case ShaderDataType::Mat2: return 4;
		case ShaderDataType::Mat3: return 9;
		case ShaderDataType::Mat4: return 16;
		}
		ASSERT(false);
		return -1;
	}
};

class BufferLayout
{
	std::vector<BufferLayoutElement> elementsMap;
	int stride;

public:
	BufferLayout(const std::initializer_list<BufferLayoutElement>& elements) : elementsMap(elements), stride(0)
	{
		for (auto& element : this->elementsMap)
		{
			element.offset = stride;
			stride += element.GetSize();
		}
	}

	inline BufferLayoutElement& operator[](int index) { return elementsMap[index]; }
	inline const BufferLayoutElement& operator[](int index) const { return elementsMap[index]; }
	inline int GetStride() const { return stride; }
	inline int GetElementCount() const { return elementsMap.size(); }
};