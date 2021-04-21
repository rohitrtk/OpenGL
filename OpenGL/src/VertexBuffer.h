#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind();
	void unbind();

	inline unsigned int getID() const { return this->ID; }
	inline void setID(unsigned int ID) { this->ID = ID; }

private:
	unsigned int ID;
};

