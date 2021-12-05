#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(float* items, int size)
	: id(0),
	  alive(true)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, items, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	if (alive)
		destroy();
}

void VertexBuffer::bind() {
	if (alive)
		glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::destroy() {
	if (alive)
	{
		glDeleteBuffers(1, &id);
		alive = false;
	}
}