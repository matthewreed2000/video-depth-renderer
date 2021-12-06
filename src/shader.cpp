#include "shader.hpp"
#include "files.hpp"

#include <glad/glad.h>
#include <stdio.h>

Shader::Shader(const char* filename, unsigned int shaderType)
	: id(0),
	  alive(true)
{
	if (files::exists(filename)) {
		std::string srcStr = files::readText(filename);

		if (srcStr.compare("") == 0)
		{
			printf("Source File Empty\n");
			throw 1;
		}
		else
			printf("Succesfully loaded '%s'\n", filename);

		id = glCreateShader(shaderType);
		const char* src = srcStr.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);
	}

	else {
		printf("Shader file '%s' could not be found\n", filename);
		alive = false;
		throw 1;
	}
}

Shader::~Shader() {
	destroy();
}

void Shader::destroy() {
	if (alive) {
		glDeleteShader(id);
		alive = false;
	}
}