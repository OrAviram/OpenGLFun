#pragma once
#include "Common.h"
#include "Tools/Math/Math.h"

enum class ShaderType
{
	Invalid = 0, Vertex = 1, TessellationControl = 2, TessellationEvaluation = 3, Geometry = 4, Fragment = 5
};

struct ShaderElement
{
	ShaderType type;
	std::string_view source;

	ShaderElement(ShaderType type, const std::string_view& source) : type(type), source(source) { }
};

class Shader
{
	uint programID;
	std::unordered_map<const char*, int> uniformLocations;

public:
	Shader(const ShaderElement* elements, int count);
	Shader(const std::initializer_list<ShaderElement>& elements);
	Shader(const char* path);
	static Shader* Load(const char* inPath);
	~Shader();

	void Bind();

	void SetUniformBoolean(const char* name, bool value);
	void SetUniformInt(const char* name, int value);
	void SetUniformUInt(const char* name, uint value);
	void SetUniformFloat(const char* name, float value);
	void SetUniformDouble(const char* name, double value);

	void SetUniformVec2(const char* name, float x, float y);
	inline void SetUniformVec2(const char* name, const Vector2f& value) { SetUniformVec2(name, value.x, value.y); }
	
	void SetUniformVec3(const char* name, float x, float y, float z);
	inline void SetUniformVec3(const char* name, const Vector3f& value) { SetUniformVec3(name, value.x, value.y, value.z); }

	void SetUniformVec4(const char* name, float x, float y, float z, float w);
	inline void SetUniformVec4(const char* name, const Vector4f& value) { SetUniformVec4(name, value.x, value.y, value.z, value.w); }
	
	void SetUniformMat4(const char* name, const Matrix4x4& value);

	void SetUniformFloatArray(const char* name, int length, const float* value);

	int GetUniformLocation(const char* name);

private:
};