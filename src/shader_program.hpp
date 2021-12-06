#pragma once

#include "shader.hpp"

#include <glm/glm.hpp>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	void add(Shader& shader);
	
	void assignUniform1i(const char* uniformName, int value);
	void assignUniform4fv(const char* uniformName, glm::mat4& value);

	void bind();
	void unbind();
	void destroy();
private:
	unsigned int id;
	bool alive;
};