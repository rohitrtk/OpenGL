#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

	unsigned int getID() const { return this->ID; }
	void setID(unsigned int ID) { this->ID = ID; }

private:
	unsigned int ID;
};

