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

		int isCompiled = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			char errorLog[maxLength];
			glGetShaderInfoLog(id, maxLength, &maxLength, errorLog);

			printf("%s\n", errorLog);

			glDeleteShader(id);
			id = 0;
		}
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