#pragma once

class TextureLoader 
{
public:
	
	static unsigned int* loadTexture(const char* path);
	static void unloadTexture(unsigned int* texture);

private:

	TextureLoader() {}
};

