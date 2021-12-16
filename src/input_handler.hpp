#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

class InputHandler {
public:
	InputHandler(GLFWwindow* window);
	~InputHandler();

	// Input Control
	void indicateFrameEnd();
	inline void attachMouse(GLFWwindow* window) { mouseAttached = true; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); };
	inline void detachMouse(GLFWwindow* window) { mouseAttached = false; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); };
	inline bool getMouseAttached() { return mouseAttached; };

	// Keyboard
	inline unsigned int getKeys() const { return keys; }
	inline bool upPressed() { return (keys & 1); };
	inline bool downPressed() { return (keys & 4); };
	inline bool leftPressed() { return (keys & 2); };
	inline bool rightPressed() { return (keys & 8); };
	inline bool spacePressed() { return (keys & 16); };
	inline bool shiftPressed() { return (keys & 32); };
	inline bool escapePressed() { return (keys & 64); };

	// Mouse
	inline double getMouseMoveX() { return (mouseAttached ? mouseX - prev_mouseX : 0); };
	inline double getMouseMoveY() { return (mouseAttached ? mouseY - prev_mouseY : 0); };
	inline double getMouseScroll() { return mouseScroll; };
	inline bool mouseClicked() { return lMousePressed; };
	inline double getMouseDragX() { return (mouseClicked() ? getMouseMoveX() : 0); };
	inline double getMouseDragY() { return (mouseClicked() ? getMouseMoveY() : 0); };
private:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	bool isMapped(int key);
	void pressKey(int key);
	void releaseKey(int key);

	// Input Control
	bool mouseAttached = false;

	// Keyboard
	unsigned int keys = 0;
	std::unordered_map<int, unsigned int> keymap;

	// Mouse
	double mouseX = 0;
	double mouseY = 0;
	double prev_mouseX = 0;
	double prev_mouseY = 0;

	double mouseScroll = 0;

	bool lMousePressed = false;
};