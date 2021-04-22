#pragma once

#include <vector>
#include <tuple>

class VertexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	void bindTextures();
	void addTexture(unsigned int* texture, unsigned int buffer);

	void setAttributes(const VertexBuffer& vbo, const std::vector<int>& attributes);

	void render();

	unsigned int getID() const { return this->ID; }
	void setID(unsigned int ID) { this->ID = ID; }

private:
	unsigned int ID;
	
	std::vector<std::tuple<unsigned int*, unsigned int>> textures;
};
