#pragma once

#include <vector>
#include <utility>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	void bindTextures();
	void addTexture(unsigned int* texture, int activeTexture);

	void setAttributes(const std::vector<int>& attributes);

	void render();

	unsigned int getID() const { return this->ID; }
	void setID(unsigned int ID) { this->ID = ID; }

private:
	unsigned int ID;

	std::vector<std::pair<unsigned int*, unsigned int>> textures;
};
