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
		if(glfwGetKey(this->window, k.first) == GLFW_PRESS && !k.second.second)
		{
			k.second.first();
			k.second.second = true;
		}

		if (glfwGetKey(this->window, k.first) == GLFW_RELEASE && k.second.second)
		{
			k.second.second = false;
		}
	}
}

void KeyboardHandler::bindKey(int key, callback function)
{
	this->keys[key] = std::make_pair(function, false);
}
