#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

std::string readText(const char* filename) {

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

GLuint compileShaderSrc(const char* filename, GLenum shaderType) {

	GLuint id;
	
	std::string srcStr = readText(filename);

	if (srcStr.compare("") == 0)
	{
		printf("Source File Empty\n");
		throw 1;
	}
	else
		printf("##########\n%s:\n\n%s\n##########", filename, srcStr.c_str());

	id = glCreateShader(shaderType);
	const char* src = srcStr.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	return id;
}

inline void resizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
	// Declaring Variables
	GLFWwindow* window;

	// Initialize GLFW
	if (!glfwInit()) {
		printf("Couldn't init GLFW\n");
		return 1;
	}

	// // Specify OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	window = glfwCreateWindow(640, 480, "Video Depth Renderer", NULL, NULL);
	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resizeWindow);

	// Initialize GLAD
	gladLoadGL();

	glViewport(0, 0, 640, 480);

	printf("%s\n", glGetString(GL_VERSION));

	// Setup Simple Mesh
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {0, 1, 2, 2, 3, 0};

	// // Setup Shader Pipeline
	unsigned int vertShader = compileShaderSrc("../res/shaders/basic.vert", GL_VERTEX_SHADER);
	unsigned int fragShader = compileShaderSrc("../res/shaders/basic.frag", GL_FRAGMENT_SHADER);

	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);
	glLinkProgram(shader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// Setup Buffers
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Unbind to Prevent Accidental Changes
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// GL Settings for Main Loop
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shader);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shader);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}