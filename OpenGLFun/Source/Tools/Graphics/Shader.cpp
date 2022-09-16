#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

static bool CheckStatus(int objectID, PFNGLGETSHADERIVPROC objectPropertyGetter, PFNGLGETSHADERINFOLOGPROC getInfoLog, GLenum statusType)
{
	int status;
	GLCall(objectPropertyGetter(objectID, statusType, &status));
	if (status == GL_TRUE)
		return true;
	else
	{
		GLCall(objectPropertyGetter(objectID, GL_INFO_LOG_LENGTH, &status));
		if (status > 0)
		{
			char* logInfo = (char*)alloca(status);
			GLCall(getInfoLog(objectID, status, NULL, logInfo));
			std::cout << logInfo << std::endl;
		}
		return false;
	}
}

static GLenum GetOpenGLShaderType(ShaderType type)
{
	switch (type)
	{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::TessellationControl:
			return GL_TESS_CONTROL_SHADER;
		case ShaderType::TessellationEvaluation:
			return GL_TESS_EVALUATION_SHADER;
		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
	}
	ASSERT(false);
	return GL_INVALID_ENUM;
}

Shader::Shader(const ShaderElement* elements, int count)
{
	GLCall(programID = glCreateProgram());

	uint* elementIDs = (uint*)alloca(count * sizeof(uint));
	for (int i = 0; i < count; i++)
	{
		const ShaderElement& element = elements[i];
		GLCall(uint elementID = glCreateShader(GetOpenGLShaderType(element.type)));
		const char* source = element.source.data();
		GLCall(glShaderSource(elementID, 1, &source, nullptr));
		GLCall(glCompileShader(elementID));

		ASSERT(CheckStatus(elementID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS));
		GLCall(glAttachShader(programID, elementID));

		elementIDs[i] = elementID;
	}

	GLCall(glLinkProgram(programID));
	ASSERT(CheckStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS));

	for (int i = 0; i < count; i++)
	{
		GLCall(glDeleteShader(elementIDs[i]));
	}

	GLCall(glValidateProgram(programID));
	ASSERT(CheckStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_VALIDATE_STATUS));
}

Shader::Shader(const std::initializer_list<ShaderElement>& elements)
{
	this->Shader::Shader(elements.begin(), elements.size());
}

static ShaderType ShaderTypeFromName(const std::string_view& name)
{
	if (name == "vertex")
		return ShaderType::Vertex;
	else if (name == "tessellationControl")
		return ShaderType::TessellationControl;
	else if (name == "tessellationEvaluation")
		return ShaderType::TessellationEvaluation;
	else if (name == "geometry")
		return ShaderType::Geometry;
	else if (name == "fragment")
		return ShaderType::Fragment;

	return ShaderType::Invalid;
}

Shader::Shader(const char* inPath)
{
	std::ifstream stream(inPath);

	std::unordered_map<ShaderType, std::string> elementsMap;
	std::string line;
	int lineNumber = 1;
	ShaderType currentElement = ShaderType::Invalid;
	while (getline(stream, line))
	{
		if (line[0] == '#')
		{
			// "#type    shader   " for example. With a bunch of spaces, which I want to support.
			int firstSpace = line.find_first_of(' ');
			if (firstSpace != -1)
			{
				std::string_view directive(line.c_str() + 1, firstSpace - 1);
				if (directive == "type")
				{
					uint i = firstSpace;
					while (line[i] == ' ' && i < line.length())
						i++;

					if (i == line.length() - 1)
					{
						std::cout << "No shader type specified on line " << lineNumber << "." << std::endl;
						ASSERT(false);
					}

					std::string_view typeName(line.c_str() + i, line.length() - i); // "shader    "
					i = typeName.find_first_of(' ');
					if (i != -1)
						typeName = std::string_view(typeName.data(), i); // "shader"

					ShaderType type = ShaderTypeFromName(typeName);
					if (type == ShaderType::Invalid)
					{
						std::cout << "Invalid shader type " << typeName << " on line " << lineNumber << "." << std::endl;
						ASSERT(false);
					}

					currentElement = type;
					continue;
				}
			}
		}

		if (currentElement == ShaderType::Invalid)
			continue;

		elementsMap[currentElement].append(line + '\n');
		lineNumber++;
	}
	
	ShaderElement* elements = Stackalloc(ShaderElement, elementsMap.size());
	int i = 0;
	for (auto& element : elementsMap)
	{
		elements[i].type = element.first;
		elements[i].source = element.second;
		i++;
	}
	this->Shader::Shader(elements, elementsMap.size());
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(programID));
}

Shader* Shader::Load(const char* path)
{
	return new Shader(path);
}

void Shader::Bind()
{
	GLCall(glUseProgram(programID));
}

void Shader::SetUniformBoolean(const char* name, bool value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformInt(const char* name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformUInt(const char* name, uint value)
{
	GLCall(glUniform1ui(GetUniformLocation(name), value));
}

void Shader::SetUniformFloat(const char* name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniformDouble(const char* name, double value)
{
	GLCall(glUniform1d(GetUniformLocation(name), value));
}

void Shader::SetUniformVec2(const char* name, float x, float y)
{
	GLCall(glUniform2f(GetUniformLocation(name), x, y));
}

void Shader::SetUniformVec3(const char* name, float x, float y, float z)
{
	GLCall(glUniform3f(GetUniformLocation(name), x, y, z));
}

void Shader::SetUniformVec4(const char* name, float x, float y, float z, float w)
{
	GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetUniformMat4(const char* name, const Matrix4x4& value)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, true, value.elements1D));
}

void Shader::SetUniformFloatArray(const char* name, int length, const float* value)
{
	GLCall(glUniform1fv(GetUniformLocation(name), length, value));
}

int Shader::GetUniformLocation(const char* name)
{
	if (uniformLocations.find(name) != uniformLocations.end())
		return uniformLocations[name];

	GLCall(int id = glGetUniformLocation(programID, name));
	if (id == -1)
		std::cout << "Can't find uniform '" << name << "'." << std::endl;

	uniformLocations[name] = id;
	return id;
}