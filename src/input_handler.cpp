#include "input_handler.hpp"

#include <iostream>

InputHandler::InputHandler(GLFWwindow* window) {
	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, keyCallback);

	keymap[25] = 1; // W
	keymap[38] = 2; // A
	keymap[39] = 4; // S
	keymap[40] = 8; // D
}

InputHandler::~InputHandler() {
	// Pass
}

bool InputHandler::isMapped(int key) {
	return (keymap.find(key) != keymap.end());
}

void InputHandler::pressKey(int key) {
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