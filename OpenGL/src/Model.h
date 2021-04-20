#pragma once

#include <glad/glad.h>
#include <vector>

using FloatVector = std::vector<GLfloat>;
using IntegerVector = std::vector<GLint>;

class Model
{
public:
	Model(const FloatVector& vertexPositions, const IntegerVector& dataIndicies, int numVerticies);

	// VAO stuff
	void genVAO();
	void bindVAO() const;

	// VBO stuff
	void genVBO();
	void bindVBO() const;

	void handleAttributes(const IntegerVector& dataIndicies, int numVerticies);

private:
	GLuint vao;
	GLuint vbo;
};
