#pragma once

#include <glad/glad.h>

class ElementBuffer {
public:
	ElementBuffer(unsigned int* items, int size);
	~ElementBuffer();

	void bind();
	void unbind();
	void destroy();
private:
	unsigned int id;
	int buffer_type;
	bool alive;
};