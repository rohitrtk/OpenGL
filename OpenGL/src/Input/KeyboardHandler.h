#pragma once

#include <GLFW/glfw3.h>

#include <map>
#include <functional>

using CallbackFunction = std::function<void()>;
/*
 * Parameters
 * bool: Is the key pressed
 * bool: Use toggle
 * CallbackFunction: Key press function
 * CallbackFunction: Key release function
 */
using KeyInfo = std::tuple<bool, bool, CallbackFunction, CallbackFunction>;
using KeyMapping = std::map<int, KeyInfo>;

class KeyboardHandler
{
public:
	KeyboardHandler(GLFWwindow* window);

	void bindKey(int key, bool toggle, const CallbackFunction& keyPressFunction, const CallbackFunction& keyReleaseFunction);
	void processInput();

private:
	GLFWwindow* window;

	KeyMapping keyBinds;
};

