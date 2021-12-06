#include <fstream>
#include <sstream>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "files.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "vertex_buffer.hpp"
#include "element_buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

#include "video_reader.hpp"

inline void resizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void displayHelp() {
	printf("VIDEO DEPTH RENDERER\n\n");
	printf("This program is meant to displace and color a plane using a\n");
	printf("regular color video file and a depth map of the same video.\n\n");
	printf("Expected: video-depth-renderer [COLOR_FILENAME] [DEPTH_FILENAME]\n\n");
	printf("COLOR_FILENAME\tThe path to the image or video that contains color data\n");
	printf("DEPTH_FILENAME\tThe path to the image or video that contains depth data\n");
}

int main(int argc, char** argv) {
	// Handle Arguments
	if (argc != 3) {
		displayHelp();
		return 1;
	}

	const char* filenameColor = argv[1];
	const char* filenameDepth = argv[2];

	if (!files::exists(filenameColor)) {
		printf("Failed to locate color file '%s'\n", filenameColor);
		return 1;
	}
	else {
		printf("Succesfully found color file '%s'\n", filenameColor);
	}

	if (!files::exists(filenameColor)) {
		printf("Failed to locate depth file '%s'\n", filenameDepth);
		return 1;
	}
	else {
		printf("Succesfully found depth file '%s'\n", filenameDepth);
	}

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
	GLFWwindow* window = glfwCreateWindow(640, 480, "Video Depth Renderer", NULL, NULL);
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

	printf("\nUsing OpenGL Verion: %s\n\n", glGetString(GL_VERSION));

	// Setup Simple Mesh
	float vertices[] = {
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 0.0f
	};
	unsigned int indices[] = {0, 1, 2, 2, 3, 0};

	// // Setup Shader Pipeline
	Shader vertShader("res/shaders/basic.vert", GL_VERTEX_SHADER);
	Shader fragShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);

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

	vao.linkAttrib<float>(vbo, 0, 3, 5 * sizeof(float), (void*)0);
	vao.linkAttrib<float>(vbo, 1, 2, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind to Prevent Accidental Changes
	vao.unbind();
	vbo.unbind();
	ebo.unbind();
	
	ebo.bind();

	// Set up video reader
	VideoReader vr(filenameColor);
	if (!vr.getSuccess()) {
		printf("Could not set up video reader\n");
		return 1;
	}

	const unsigned int frameWidth = vr.getWidth();
	const unsigned int frameHeight = vr.getHeight();

	unsigned char* frame = nullptr;
	vr.readFrame(&frame);

	// Set up texture for frames
	Texture texture(0);
	texture.assignBuffer(frame, vr.getWidth(), vr.getHeight());
	shaderProgram.assignUniform1i("tex0", 0);

	texture.bind();

	// GL Settings for Main Loop
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// More Variables for Main Loop
	glm::mat4 frameScale;
	if (frameWidth > frameHeight)
		frameScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, (float)frameHeight/frameWidth, 1.0f));
	else
		frameScale = glm::scale(glm::mat4(1.0f), glm::vec3((float)frameWidth/frameHeight, 1.0f, 1.0f));

	glm::mat4 windowScale = glm::mat4(1.0f);

	int width = 0;
	int height = 0;
	int prevWidth = 0;
	int prevHeight = 0;

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Change scale if window resizes
		glfwGetFramebufferSize(window, &width, &height);
		if((width != prevWidth) || (height != prevHeight)) {
			prevWidth = width;
			prevHeight = height;

			if (width > height)
				windowScale = glm::scale(glm::mat4(1.0f), glm::vec3((float)height/width, 1.0f, 1.0f));
			else
				windowScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, (float)width/height, 1.0f));
		}

		shaderProgram.assignUniform4fv("u_WindowScale", windowScale);
		shaderProgram.assignUniform4fv("u_FrameScale", frameScale);

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