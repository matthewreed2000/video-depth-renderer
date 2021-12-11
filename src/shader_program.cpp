#include "shader_program.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

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

void ShaderProgram::assignUniform1i(const char* uniformName, int value) {
	bind();
	unsigned int location = glGetUniformLocation(id, uniformName);
	glUniform1i(location, value);
	unbind();
}

void ShaderProgram::assignUniform1f(const char* uniformName, float value) {
	bind();
	unsigned int location = glGetUniformLocation(id, uniformName);
	glUniform1f(location, value);
	unbind();
}

void ShaderProgram::assignUniform4fv(const char* uniformName, glm::mat4& value) {
	bind();
	unsigned int location = glGetUniformLocation(id, uniformName);
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
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