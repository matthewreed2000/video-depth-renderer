#pragma once

#include "shader_program.hpp"

class Texture {
public:
	Texture(int slotNum);
	~Texture();

	void assignBuffer(unsigned char* data, int width, int height);
	void bind();
	void unbind();
private:
	unsigned int slot;
	unsigned int id;
	bool alive = false;
};