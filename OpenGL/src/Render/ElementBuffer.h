#pragma once

class ElementBuffer
{
public:
	ElementBuffer(const void* data, unsigned int size);
	~ElementBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int ID;
};

