#include "texture.hpp"

Texture::Texture(int slotNum) {
	slot = slotNum;

	glGenTextures(1, &id);
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	alive = true;
	// unbind();
}

Texture::~Texture() {
	unbind();
	glDeleteTextures(1, &id);
}

void Texture::assignBuffer(unsigned char* data, int width, int height, unsigned int outType) {
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, outType, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	unbind();
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}