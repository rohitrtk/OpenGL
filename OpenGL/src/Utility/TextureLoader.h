#pragma once

class TextureLoader 
{
public:
	TextureLoader() = delete;

	static unsigned int* loadTexture(const char* path, bool transparent);
	static void unloadTexture(unsigned int* texture);
};

