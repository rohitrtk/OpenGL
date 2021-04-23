#include "TextureLoader.h"

#include <glad/glad.h>
#include "stb_image.h"

#include <iostream>

#include "Debug.h"

unsigned int* TextureLoader::loadTexture(const char* path, bool transparent)
{
	unsigned int* texture = new unsigned int;
	
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, ncChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &width, &height, &ncChannels, NULL);
	if (data) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, transparent ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture from " << path << std::endl;
	}

	stbi_image_free(data);

	LOG("Successfully loaded image from path " << path);
	
	return texture;
}

void TextureLoader::unloadTexture(unsigned int* texture)
{
	glDeleteTextures(1, texture);

	if (texture != nullptr)
	{
		delete texture;
	}
}
