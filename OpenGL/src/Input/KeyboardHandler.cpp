#include "KeyboardHandler.h"

KeyboardHandler::KeyboardHandler(GLFWwindow* window) :
	window(window)
{	
}

void KeyboardHandler::bindKey(int key, bool toggle, const CallbackFunction& keyPressFunction, const CallbackFunction& keyReleaseFunction)
{
	this->keyBinds[key] = std::make_tuple(false, toggle, keyPressFunction, keyReleaseFunction);
}

void KeyboardHandler::processInput()
{
	for(auto& k : this->keyBinds)
	{
		auto& key = k.first;
		auto& pressed = std::get<0>(k.second);
		auto& toggle = std::get<1>(k.second);
		auto& keyPressFunction = std::get<2>(k.second);
		auto& keyReleaseFunction = std::get<3>(k.second);

		if(!toggle)
		{
			if(glfwGetKey(this->window, key) == GLFW_PRESS)
			{
				pressed = true;
				keyPressFunction();
			}
			else if(glfwGetKey(this->window, key) == GLFW_RELEASE)
			{
				pressed = false;
				if (keyReleaseFunction) keyReleaseFunction();
			}
			
			continue;
		}

		if (glfwGetKey(this->window, key) == GLFW_PRESS && !pressed)
		{
			pressed = true;
			keyPressFunction();
		}
		else if (glfwGetKey(this->window, key) == GLFW_RELEASE && pressed)
		{
			pressed = false;
			if (keyReleaseFunction) keyReleaseFunction();
		}
	}
}

