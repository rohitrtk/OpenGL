#include "KeyboardHandler.h"



KeyboardHandler::KeyboardHandler(GLFWwindow* window)
	: window(window)
{
	
}

void KeyboardHandler::processInput()
{
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, true);
	}
	
	for(auto& k : this->keys)
	{
		
	}
}

void KeyboardHandler::bindKey(int key, callback keyPressFunction, callback keyReleaseFunction, bool ignorePressRelease)
{
	this->keys[key] = std::make_tuple(keyPressFunction, keyReleaseFunction, false, ignorePressRelease);
}
