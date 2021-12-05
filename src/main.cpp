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
		// throw 1;
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	window = glfwCreateWindow(640, 480, "Video Depth Renderer", NULL, NULL);
	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	// glfwSetWindowSizeCallback(window, resizeWindow);

	// Initialize GLAD
	gladLoadGL();

	glViewport(0, 0, 640, 480);

	printf("%s\n", glGetString(GL_VERSION));

	// Setup Simple Mesh
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// // Setup Shader Pipeline
	// const char *vertexShaderSource = "#version 330 core\n"
	//     "layout (location = 0) in vec3 aPos;\n"
	//     "void main()\n"
	//     "{\n"
	//     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	//     "}\0";

	// const char *fragmentShaderSource = "#version 330 core\n"
	// 	"out vec4 FragColor;\n"
	// 	"\n"
	// 	"void main()\n"
	// 	"{\n"
	// 	"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	// 	"}\0";

	unsigned int vertShader = compileShaderSrc("../res/shaders/basic.vert", GL_VERTEX_SHADER);
	unsigned int fragShader = compileShaderSrc("../res/shaders/basic.frag", GL_FRAGMENT_SHADER);

 	// unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
	// glCompileShader(vertShader);

	// unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	// glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	// glCompileShader(fragShader);

	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);
	glLinkProgram(shader);
	// glValidateProgram(shader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// Setup Buffers
	unsigned int vao;
	unsigned int vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind to Prevent Accidental Changes
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// GL Settings for Main Loop
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shader);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBegin(GL_TRIANGLES);
		// glVertex2f(-0.5f, -0.5f);
		// glVertex2f( 0.0f,  0.5f);
		// glVertex2f( 0.5f, -0.5f);
		// glEnd();

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();

		// // Specify the color of the background
		// glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// // Clean the back buffer and assign the new color to it
		// glClear(GL_COLOR_BUFFER_BIT);
		// // Tell OpenGL which Shader Program we want to use
		// glUseProgram(shader);
		// // Bind the VAO so OpenGL knows to use it
		// glBindVertexArray(vao);
		// // Draw the triangle using the GL_TRIANGLES primitive
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// // Swap the back buffer with the front buffer
		// glfwSwapBuffers(window);
		// // Take care of all GLFW events
		// glfwPollEvents();
	}

	glDeleteProgram(shader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}