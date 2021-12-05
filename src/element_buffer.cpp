#include "element_buffer.hpp"

ElementBuffer::ElementBuffer(unsigned int* items, int size)
	: id(0),
	  alive(true)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, items, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer() {
	if (alive)
		destroy();
}

void ElementBuffer::bind() {
	if (alive)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ElementBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::destroy() {
	if (alive)
	{
		glDeleteBuffers(1, &id);
		alive = false;
	}
}