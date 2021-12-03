#include <iostream>
#include <fstream>
#include <sstream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

GLuint compileShader(const char* filename, GLenum type) {
	std::fstream stream(filename);

	std::string line;
	std::stringstream ss;
}

inline void resizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
	// Declaring Variables
	GLFWwindow* window;

	// Initialize GLFW
	if (!glfwInit()) {
		std::cout << "Couldn't init GLFW\n";
		return 1;
	}

	// Specify OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Create Window
	window = glfwCreateWindow(640, 480, "Video Depth Renderer", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resizeWindow);

	// Setup Simple Quad
	GLfloat vertices[] = {
		-0.5, -0.5,  0.5, 1.0, 0.0, 0.0, 0.0, 0.0,
		 0.5, -0.5,  0.5, 1.0, 0.0, 0.0, 1.0, 0.0,
		 0.5,  0.5,  0.5, 1.0, 0.0, 0.0, 1.0, 1.0,
		 -0.5,  0.5,  0.5, 1.0, 0.0, 0.0, 0.0, 1.0};

	GLuint indices[] = {0, 1, 2, 3};

	// Setup Pipeline
	GLuint vertShader = compileShader("res/shader/basic.vert", GL_VERTEX_SHADER);
	GLuint tessShader = compileShader("res/shader/basic.vert", GL_TESS_CONTROL_SHADER);
	GLuint evalShader = compileShader("res/shader/basic.vert", GL_TESS_EVALUATION_SHADER);
	GLuint fragShader = compileShader("res/shader/basic.vert", GL_FRAGMENT_SHADER);

	// Allow for correct depth rendering
	glEnable(GL_DEPTH_TEST);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}