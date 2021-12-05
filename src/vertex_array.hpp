#pragma once

#include "vertex_buffer.hpp"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	template <typename T>
	void linkAttrib(VertexBuffer& vbo, unsigned int layout, unsigned int numComponenets, int stride, void* offset);
	void linkAttrib(VertexBuffer& vbo, unsigned int layout, unsigned int numComponenets, unsigned int dataType, int stride, void* offset);
private:
	unsigned int id;
};