#pragma once

#include <vector>

class VertexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	void bindTexture();
	void setTexture(unsigned int* texture);

	void setAttributes(const VertexBuffer& vbo, const std::vector<int>& attributes);

	void render();

	inline unsigned int getID() const { return this->ID; }
	inline void setID(unsigned int ID) { this->ID = ID; }

private:
	unsigned int ID;
	unsigned int* texture;
};
