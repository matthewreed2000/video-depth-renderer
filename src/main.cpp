#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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