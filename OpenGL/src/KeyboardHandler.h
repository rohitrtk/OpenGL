#pragma once

#include <GLFW/glfw3.h>

#include <map>
#include <tuple>

typedef void(*callback)();

// First in the map represents the key
// First in the tuple represents the function call when the key is pressed
// Second in the tuple represents if the key is currently pressed
// Third in the tuple represents if we should ignore if the key is currently pressed
using BindArguments = std::tuple<callback, callback, bool, bool>;

enum KeyNavigatorIndex
{
	KeyPressFunction = 0,
	KeyReleaseFunction,
	IsPressed,
	IgnorePressRelease
};

using KNI = KeyNavigatorIndex;

class KeyboardHandler
{
public:
	KeyboardHandler(GLFWwindow* window);

	void processInput();
	void bindKey(int key, callback keyPressFunction, callback keyReleaseFunction, bool ignorePressRelease);

private:
	GLFWwindow* window;

	std::map<int, BindArguments> keys;
};

