#include "Common.h"
#include <GL/glew.h>
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* functionName, const char* file, int line)
{
	bool clean = true;
	while (GLenum error = glGetError())
	{
		std::cout << "GL Error on function " << functionName << ": " << error << " (file " << file << ", line " << line << ")"<< std::endl;
		clean = false;
	}
	return clean;
}

void Utils::Replace(std::string& source, const char* target, const char* newValue)
{
	size_t location = 0;
	while ((location = source.find(target, location)) != std::string::npos)
	{
		source.replace(location, strlen(target), newValue);
		location += strlen(newValue);
	}
}