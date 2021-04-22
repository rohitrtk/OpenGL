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
		for(const auto& t : this->textures)
		{
			glActiveTexture(std::get<1>(t));
			glBindTexture(GL_TEXTURE_2D, *(std::get<0>(t)));
			
			//glBindTexture(GL_TEXTURE_2D, *(this->textures[0]));
		}
	}
}

void VertexArray::addTexture(unsigned int* texture, unsigned int buffer)
{
	this->textures.emplace_back(std::make_tuple(texture, buffer));
}

void VertexArray::setAttributes(const VertexBuffer& buffer, const std::vector<int>& attributes)
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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	this->unbind();
}

