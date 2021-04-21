#include "VertexArray.h"

#include <glad/glad.h>

#include "VertexBuffer.h"

VertexArray::VertexArray()
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

void VertexArray::bindTexture()
{
	if (this->texture)
	{
		glBindTexture(GL_TEXTURE_2D, *(this->texture));
	}
}

void VertexArray::setTexture(unsigned int* texture)
{
	this->texture = texture;
}

void VertexArray::setAttributes(const VertexBuffer& buffer, const std::vector<int>& attributes)
{
	int p = 0;
	int sob = 0;
	int verticies = 8;
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
	this->bindTexture();

	this->bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	this->unbind();
}

