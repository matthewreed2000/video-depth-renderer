#include "shader_program.hpp"

#include <glad/glad.h>

ShaderProgram::ShaderProgram() {
	id = glCreateProgram();
	alive = true;
}

ShaderProgram::~ShaderProgram() {
	destroy();
}

void ShaderProgram::add(Shader& shader) {

	bind();
	glAttachShader(id, shader.getID());
	glLinkProgram(id);
	unbind();
}

void ShaderProgram::bind() {
	glUseProgram(id);
}

void ShaderProgram::unbind() {
	glUseProgram(0);
}

void ShaderProgram::destroy() {
	if (alive) {
		glDeleteProgram(id);
		alive = false;
	}
}