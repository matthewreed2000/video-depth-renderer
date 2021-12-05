#include "buffers.hpp"

template <typename T>
Buffer<T>::Buffer(int buffer_type, T* items, int size)
	: buffer_type(buffer_type),
	  id(0),
	  alive(true)
{
	glGenBuffers(1, &id);
	glBindBuffer(buffer_type, id);
	glBufferData(buffer_type, size, items, GL_STATIC_DRAW);
}

template <typename T>
Buffer<T>::~Buffer() {
	if (alive)
		destroy();
}

template <typename T>
void Buffer<T>::bind() {
	if (alive)
		glBindBuffer(buffer_type, id);
}

template <typename T>
void Buffer<T>::unbind() {
	glBindBuffer(buffer_type, 0);
}

template <typename T>
void Buffer<T>::destroy() {
	if (alive)
	{
		glDeleteBuffers(1, &id);
		alive = false;
	}
}