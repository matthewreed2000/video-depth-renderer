#pragma once

#include "shader.hpp"
#include <initializer_list>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	void add(Shader& shader);
	void bind();
	void unbind();
	void destroy();
private:
	unsigned int id;
	bool alive;
};