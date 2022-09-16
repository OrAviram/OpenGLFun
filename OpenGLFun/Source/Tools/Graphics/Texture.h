#pragma once
#include "Common.h"

class Texture
{
	uint id;
	int width, height, bpp;

public:
	Texture(const char* filePath, uint initialBindingSlot = 0);
	~Texture();
	static Texture* Load(const char* filePath, uint initialBindingSlot = 0);

	void Bind(uint slot);
	void Unbind();

	inline int Width() { return width; }
	inline int Height() { return height; }
	inline int BPP() { return bpp; }
};