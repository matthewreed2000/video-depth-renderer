#include <fstream>
#include <sstream>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "input_handler.hpp"

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
	printf("Expected: video-depth-renderer COLOR_FILENAME [DEPTH_FILENAME]\n\n");
	printf("COLOR_FILENAME\tThe path to the image or video that contains color data\n");
	printf("DEPTH_FILENAME\t(OPTIONAL) The path to the image or video that contains depth data\n");
}

int main(int argc, char** argv) {
	// Handle Arguments
	if (argc < 2 || argc > 3) {
		displayHelp();
		return 1;
	}

	bool depthDefined = (argc == 3);

	const char* filenameColor = argv[1];
	const char* filenameDepth = (depthDefined ? argv[2] : nullptr);

	// Initialize GLFW
	if (!glfwInit()) {
		printf("Couldn't init GLFW\n");
		return 1;
	}

	// // Specify OpenGL Version
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(640, 480, "Video Depth Renderer", NULL, NULL);
	if (!window) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	InputHandler ih(window);

	glfwSetWindowSizeCallback(window, resizeWindow);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLAD
	gladLoadGL();

	glViewport(0, 0, 640, 480);

	printf("\nUsing OpenGL Verion: %s\n\n", glGetString(GL_VERSION));

	// Setup Simple Mesh
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // 0 Bottom Left
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // 1 Bottom Right
		 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // 2 Top Right
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // 3 Top Left

		 0.0f, -1.0f, 0.0f, 0.5f, 1.0f, // 4 Bottom
		 0.0f,  0.0f, 0.0f, 0.5f, 0.5f, // 5 Center
		-1.0f,  0.0f, 0.0f, 0.0f, 0.5f, // 6 Left
		 1.0f,  0.0f, 0.0f, 1.0f, 0.5f, // 7 Right
		 0.0f,  1.0f, 0.0f, 0.5f, 0.0f, // 8 Top
	};
	// unsigned int indices[] = {0, 1, 2, 2, 3, 0};
	// unsigned int indices[] = {0, 1, 2, 3};
	unsigned int indices[] = {
		0, 4, 5, 6,
		4, 1, 7, 5,
		6, 5, 8, 3,
		5, 7, 2, 8
	};

	// Setup Shader Pipeline

	Shader* vertShader = nullptr;
	Shader* tessShader = nullptr;
	Shader* evalShader = nullptr;
	Shader* fragShader = nullptr;

	if (depthDefined) {
		vertShader = new Shader("../res/shaders/depthTess.vert", GL_VERTEX_SHADER);
		tessShader = new Shader("../res/shaders/depthTess.tcs", GL_TESS_CONTROL_SHADER);
		evalShader = new Shader("../res/shaders/depthTess.tes", GL_TESS_EVALUATION_SHADER);
		fragShader = new Shader("../res/shaders/depthTess.frag", GL_FRAGMENT_SHADER);
	}
	else {
		vertShader = new Shader("../res/shaders/combinedTess.vert", GL_VERTEX_SHADER);
		tessShader = new Shader("../res/shaders/combinedTess.tcs", GL_TESS_CONTROL_SHADER);
		evalShader = new Shader("../res/shaders/combinedTess.tes", GL_TESS_EVALUATION_SHADER);
		fragShader = new Shader("../res/shaders/combinedTess.frag", GL_FRAGMENT_SHADER);	
	}

	ShaderProgram shaderProgram;
	shaderProgram.add(*vertShader);
	shaderProgram.add(*tessShader);
	shaderProgram.add(*evalShader);
	shaderProgram.add(*fragShader);

	vertShader->destroy();
	tessShader->destroy();
	evalShader->destroy();
	fragShader->destroy();

	delete vertShader;
	delete tessShader;
	delete evalShader;
	delete fragShader;
	vertShader = nullptr;
	tessShader = nullptr;
	evalShader = nullptr;
	fragShader = nullptr;

	// Setup Buffers
	glPatchParameteri(GL_PATCH_VERTICES, 4);

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
	VideoReader colorVR(filenameColor);
	VideoReader depthVR(filenameDepth);
	// if (!colorVR.getSuccess()) {
	// 	printf("Could not set up video reader\n");
	// 	return 1;
	// }

	unsigned char* colorFrame = nullptr;
	unsigned char* depthFrame = nullptr;
	colorVR.readFrame(&colorFrame);
	depthVR.readFrame(&depthFrame);

	const unsigned int frameWidth = (depthDefined ? colorVR.getWidth() : colorVR.getWidth() / 2);
	const unsigned int frameHeight = colorVR.getHeight();

	// Set up texture for frames
	Texture colorTexture(0);
	colorTexture.assignBuffer(colorFrame, colorVR.getWidth(), colorVR.getHeight());
	shaderProgram.assignUniform1i("tex0", 0);

	Texture depthTexture(1);
	depthTexture.assignBuffer(depthFrame, depthVR.getWidth(), depthVR.getHeight(), GL_RED);
	shaderProgram.assignUniform1i("tex1", 1);

	depthTexture.bind();
	colorTexture.bind();

	// GL Settings for Main Loop
	glClearColor(0.1f, 0.2f, 0.15f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// More Variables for Main Loop
	int width = 0;
	int height = 0;
	int prevWidth = 0;
	int prevHeight = 0;

	float rotationX = 0.0f;
	float rotationY = 0.0f;
	float depth = 0.5f;

	float speed = 0.1f;

	glm::vec4 pos = glm::vec4(0.0f, 0.0f, -4.0f, 1.0f);

	glm::mat4 mScale;
	if (frameWidth > frameHeight)
		mScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, (float)frameHeight/frameWidth, 1.0f));
	else
		mScale = glm::scale(glm::mat4(1.0f), glm::vec3((float)frameWidth/frameHeight, 1.0f, 1.0f));

	glm::mat4 model = glm::mat4(1.0f);
	
	glm::mat4 vRotation = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 proj = glm::mat4(1.0f);
	glm::mat4 mvp = glm::mat4(1.0f);

	// Make sure the video plays back at the right speed
	glfwSetTime(0.0);
	int64_t ptsColor = 0;
	int64_t ptsDepth = 0;

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Change scale if window resizes
		glfwGetFramebufferSize(window, &width, &height);
		if((width != prevWidth) || (height != prevHeight)) {
			prevWidth = width;
			prevHeight = height;
			proj = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);
		}

		// Read color frame at speed
		// if (ptsColor != 0) {
			if (glfwGetTime() > ptsColor * (double)colorVR.getTimeBase().num / (double)colorVR.getTimeBase().den)
			{
				// Read new frame
				if (!colorVR.readFrame(&colorFrame, &ptsColor)) {
					printf("Couldn't load video frame\n");
					return 1;
				}
				colorTexture.assignBuffer(colorFrame, colorVR.getWidth(), colorVR.getHeight());
				colorTexture.bind();
			}
		// }

		// Read depth frame at speed
		if (depthDefined && ptsColor != 0) {
			if (glfwGetTime() > ptsDepth * (double)depthVR.getTimeBase().num / (double)depthVR.getTimeBase().den)
			{
				// Read new frame
				if (!depthVR.readFrame(&depthFrame, &ptsDepth)) {
					printf("Couldn't load video frame\n");
					return 1;
				}
				depthTexture.assignBuffer(depthFrame, depthVR.getWidth(), depthVR.getHeight());
				depthTexture.bind();
			}
		}

		// shaderProgram.assignUniform4fv("u_WindowScale", windowScale);
		// shaderProgram.assignUniform4fv("u_FrameScale", frameScale);


		// First, scale the model to match aspect ratio
		// glm::mat4 model = frameScale;
		// glm::mat4 view = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
		// glm::mat4 proj = windowScale * defaultProj;
		// glm::mat4 proj = defaultProj;


		if (ih.upPressed())
			pos = pos + glm::rotate(glm::mat4(1.0f), glm::radians(-rotationX), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, speed, 0.0f);
		if (ih.downPressed())
			pos = pos + glm::rotate(glm::mat4(1.0f), glm::radians(-rotationX), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -speed, 0.0f);
		if (ih.leftPressed())
			pos = pos + glm::rotate(glm::mat4(1.0f), glm::radians(-rotationX), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(speed, 0.0f, 0.0f, 0.0f);
		if (ih.rightPressed())
			pos = pos + glm::rotate(glm::mat4(1.0f), glm::radians(-rotationX), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(-speed, 0.0f, 0.0f, 0.0f);
		if (ih.spacePressed())
			pos = pos + glm::vec4(0.0f, -speed, 0.0f, 0.0f);
		if (ih.shiftPressed())
			pos = pos + glm::vec4(0.0f, speed, 0.0f, 0.0f);

		if (ih.getMouseScroll() > 0)
			depth += 0.01;
		if (ih.getMouseScroll() < 0)
			depth -= 0.01;

		if (ih.mouseClicked())
			ih.attachMouse(window);
		if (ih.escapePressed())
			ih.detachMouse(window);

		rotationX += (ih.getMouseMoveX() / width) * 360;
		rotationY += (ih.getMouseMoveY() / height) * 360;

		model = mScale;

		vRotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		vRotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationY), glm::vec3(1.0f, 0.0f, 0.0f)) * vRotation;

		view = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
		view = vRotation * view;

		mvp = proj * view * model;

		shaderProgram.assignUniform1f("u_Depth", depth);
		shaderProgram.assignUniform4fv("u_MVP", mvp);

		shaderProgram.bind();
		vao.bind();
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		// glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, nullptr);
		// glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_INT, nullptr);
		glDrawElements(GL_PATCHES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);

		// Update callback-less values
		ih.indicateFrameEnd();

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}