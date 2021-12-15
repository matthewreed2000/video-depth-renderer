#include "input_handler.hpp"

#include <iostream>

InputHandler::InputHandler(GLFWwindow* window) {
	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);

	keymap[25] = 1; // W
	keymap[38] = 2; // A
	keymap[39] = 4; // S
	keymap[40] = 8; // D
	keymap[9] = 16; // ESC
}

InputHandler::~InputHandler() {
	// Pass
}

void InputHandler::indicateFrameEnd() {
	mouseScroll = 0;
	prev_mouseX = mouseX;
	prev_mouseY = mouseY;
}

bool InputHandler::isMapped(int key) {
	return (keymap.find(key) != keymap.end());
}

void InputHandler::pressKey(int key) {
	std::cout << key << std::endl;
	if (isMapped(key))
		keys = keys | keymap[key];
}

void InputHandler::releaseKey(int key) {
	if (isMapped(key))
		keys = keys & ~keymap[key];
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	InputHandler* ih = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
		ih->pressKey(scancode);
	else if (action == GLFW_RELEASE)
		ih->releaseKey(scancode);
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	InputHandler* ih = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			ih->lMousePressed = true;
		}
		else if (action == GLFW_RELEASE) {
			ih->lMousePressed = false;
		}
	}

}

void InputHandler::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	InputHandler* ih = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	ih->mouseX = xpos;
	ih->mouseY = ypos;
}

void InputHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	InputHandler* ih = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

	ih->mouseScroll = yoffset;
}