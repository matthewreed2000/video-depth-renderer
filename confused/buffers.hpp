#pragma once

#include <glad/glad.h>

template <typename T>
class Buffer {
public:
	Buffer(int buffer_type, T* items, int size);
	~Buffer();

	void bind();
	void unbind();
	void destroy();
protected:
	unsigned int id;
	int buffer_type;
	bool alive;
};

class VertexBuffer: public Buffer<float> {
public:
	VertexBuffer(float* items, int size) : Buffer<float>(GL_ARRAY_BUFFER, items, size) { }
};

class ElementBuffer: public Buffer<unsigned int> {
public:
	ElementBuffer(unsigned int* items, int size) : Buffer<unsigned int>(GL_ARRAY_BUFFER, items, size) { }
};