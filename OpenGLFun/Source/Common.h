#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <initializer_list>

typedef unsigned int uint;
typedef unsigned char byte;
typedef char sbyte;

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* functionName, const char* file, int line);

#define Stackalloc(T, count) ((T*)alloca((count) * sizeof(T)))

namespace Utils
{
	void Replace(std::string& source, const char* target, const char* newValue);
}