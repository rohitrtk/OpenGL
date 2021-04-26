#pragma once

#include <GLFW/glfw3.h>

#include <map>
#include <utility>

typedef void(*callback)();

// For clarity
typedef int keyCode;
typedef int keyMode;
typedef bool pressed;

class KeyboardHandler
{
public:
	KeyboardHandler(GLFWwindow* window);

	void processInput();
	void bindKey(int key, callback function);

private:
	GLFWwindow* window;

	std::map<keyCode, std::pair<callback, pressed>> keys;
};

