#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

class InputHandler {
public:
	InputHandler(GLFWwindow* window);
	~InputHandler();

	inline unsigned int getKeys() const { return keys; }
	inline bool upPressed() { return (keys & 1); };
	inline bool downPressed() { return (keys & 4); };
	inline bool leftPressed() { return (keys & 2); };
	inline bool rightPressed() { return (keys & 8); };
private:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool isMapped(int key);
	void pressKey(int key);
	void releaseKey(int key);

	unsigned int keys = 0;
	std::unordered_map<int, unsigned int> keymap;
};