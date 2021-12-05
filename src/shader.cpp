#include "shader.hpp"

#include <glad/glad.h>

Shader::Shader(const char* filename, unsigned int shaderType)
	: id(0),
	  alive(true)
{
	std::string srcStr = readText(filename);

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

Shader::~Shader() {
	destroy();
}

void Shader::destroy() {
	if (alive) {
		glDeleteShader(id);
		alive = false;
	}
}

std::string Shader::readText(const char* filename) {

	std::fstream f(filename);

	if (!f.good())
	{
		printf("Failed to open file %s\n", filename);
	}

	std::string line;
	std::stringstream ss;

	while (getline(f, line)) {
		ss << line << '\n';
	}

	return ss.str();
}