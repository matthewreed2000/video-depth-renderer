#pragma once

#include "shader_program.hpp"
#include <glad/glad.h>

class Texture {
public:
	Texture(int slotNum);
	~Texture();

	void assignBuffer(unsigned char* data, int width, int height, unsigned int outType=GL_RGB);
	void bind();
	void unbind();
private:
	unsigned int slot;
	unsigned int id;
	bool alive = false;
};