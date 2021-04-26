#include "VertexArray.h"

#include <glad/glad.h>

#include "VertexBuffer.h"

VertexArray::VertexArray()
	: ID(0)
{
	glGenVertexArrays(1, &(this->ID));
	glBindVertexArray(this->ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &(this->ID));
}

void VertexArray::bind()
{
	glBindVertexArray(this->ID);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::bindTextures()
{
	if (!this->textures.empty())
	{
		for(const auto& texture : this->textures)
		{
			glActiveTexture(texture.second);
			glBindTexture(GL_TEXTURE_2D, *(texture.first));
		}
	}
}

void VertexArray::addTexture(unsigned int* texture, int activeTexture)
{
	this->textures.emplace_back(std::make_pair(texture, activeTexture));
}

void VertexArray::setAttributes(const std::vector<int>& attributes)
{
	int p = 0;
	int sob = 0;
	int verticies = 0;
	for(const auto& v : attributes)
	{
		verticies += v;
	}
	const int sof = sizeof(float);

	for (unsigned int i = 0; i < attributes.size(); i++)
	{
		sob = attributes.at(i);
		glVertexAttribPointer(i, sob, GL_FLOAT, GL_FALSE, 
			verticies * sof, (void*)(p * sof));
		glEnableVertexAttribArray(i);

		p += sob;
	}
}

void VertexArray::render()
{
	this->bindTextures();

	this->bind();
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	this->unbind();
}

