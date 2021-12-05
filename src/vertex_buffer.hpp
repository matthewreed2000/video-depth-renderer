#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
	VertexBuffer(float* items, int size);
	~VertexBuffer();

	void bind();
	void unbind();
	void destroy();
private:
	unsigned int id;
	int buffer_type;
	bool alive;
};