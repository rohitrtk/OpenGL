#include "Model.h"

Model::Model(const FloatVector& vertexPositions, const IntegerVector& dataIndicies, int numVerticies) 
{
	this->vao = NULL;
	this->genVAO();
	this->bindVAO();

	this->vbo = NULL;
	this->genVBO();
	this->bindVBO();

	this->handleAttributes(dataIndicies, numVerticies);
}

void Model::genVAO()
{
	glGenVertexArrays(1, &this->vao);
}

void Model::bindVAO() const
{
	glBindVertexArray(this->vao);
}

void Model::genVBO()
{
	glGenBuffers(1, &this->vbo);
}

void Model::bindVBO() const
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
}

void Model::handleAttributes(const IntegerVector& dataIndicies, int numVerticies) 
{
	int p = 0;
	for (int i = 0; i < dataIndicies.size(); i++) 
	{
		glVertexAttribPointer(i, dataIndicies.at(i), GL_FLOAT, GL_FALSE,
			numVerticies * sizeof(GL_FLOAT), (void*)(p * sizeof(float)));
		p += dataIndicies.at(i);
	}
}
