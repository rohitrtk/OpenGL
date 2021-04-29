#include "ElementBuffer.h"

#include <glad/glad.h>

ElementBuffer::ElementBuffer(const void* data, unsigned int size)
	: ID(0)
{
	glGenBuffers(1, &(this->ID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &(this->ID));
}

void ElementBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
}

void ElementBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
