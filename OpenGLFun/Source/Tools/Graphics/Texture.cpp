#include "Texture.h"
#include "Vendor/STBImage/stb_image.h"
#include <GL/glew.h>

Texture::Texture(const char* filePath, uint initialBindingSlot)
{
	stbi_set_flip_vertically_on_load(1);
	byte* data = stbi_load(filePath, &width, &height, &bpp, 4);

	GLCall(glGenTextures(1, &id));
	GLCall(glActiveTexture(GL_TEXTURE0 + initialBindingSlot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

	if (data)
		stbi_image_free(data);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &id));
}

Texture* Texture::Load(const char* filePath, uint initialBindingSlot)
{
	return new Texture(filePath, initialBindingSlot);
}

void Texture::Bind(uint slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}