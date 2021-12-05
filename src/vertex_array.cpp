#include "vertex_array.hpp"

VertexArray::VertexArray() {

	glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {

	glDeleteVertexArrays(1, &id);
}

void VertexArray::bind() {
	glBindVertexArray(id);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

void VertexArray::linkAttrib(VertexBuffer& vbo, unsigned int layout, unsigned int numComponenets, int stride, void* offset) {
	vbo.bind();
	glVertexAttribPointer(layout, numComponenets, GL_FLOAT, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}