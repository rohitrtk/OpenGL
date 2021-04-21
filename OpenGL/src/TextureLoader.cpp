#include "TextureLoader.h"

#include <glad/glad.h>
#include "stb_image.h"

#include <iostream>

unsigned int* TextureLoader::loadTexture(const char* path)
{
	unsigned int* texture = new unsigned int;
	
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, ncChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &width, &height, &ncChannels, NULL);
	if (data) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture from " << path << std::endl;
	}

	stbi_image_free(data);

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
