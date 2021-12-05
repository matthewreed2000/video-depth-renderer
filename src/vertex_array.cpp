#include <stdio.h>
#include <typeinfo>

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

void VertexArray::linkAttrib(VertexBuffer& vbo, unsigned int layout, unsigned int numComponenets, unsigned int dataType, int stride, void* offset) {
	vbo.bind();
	glVertexAttribPointer(layout, numComponenets, dataType, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

template <typename T>
void VertexArray::linkAttrib(VertexBuffer& vbo, unsigned int layout, unsigned int numComponenets, int stride, void* offset) {
	printf("Unsupported VertexBuffer type: '%s'. Defaulting to unsigned bytes.", typeid(T).name());

	linkAttrib(vbo, layout, numComponenets, GL_UNSIGNED_BYTE, stride, offset);
}

template <>
void VertexArray::linkAttrib<float>(VertexBuffer& vbo, unsigned int layout, unsigned int numComponenets, int stride, void* offset) {
	linkAttrib(vbo, layout, numComponenets, GL_FLOAT, stride, offset);
}