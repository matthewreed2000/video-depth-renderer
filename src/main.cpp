#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "shader_program.hpp"
#include "vertex_buffer.hpp"
#include "element_buffer.hpp"
#include "vertex_array.hpp"

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
	Shader vertShader("../res/shaders/basic.vert", GL_VERTEX_SHADER);
	Shader fragShader("../res/shaders/basic.frag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderProgram;
	shaderProgram.add(vertShader);
	shaderProgram.add(fragShader);

	vertShader.destroy();
	fragShader.destroy();

	// Setup Buffers
	VertexArray vao;
	vao.bind();

	VertexBuffer vbo(vertices, sizeof(vertices));
	vbo.bind();

	ElementBuffer ebo(indices, sizeof(indices));
	ebo.bind();

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
	
	ebo.bind();

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.bind();
		vao.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}