#include <core/InputManager.h>
#include "GLFW/glfw3.h"

namespace teliod::core
{
	InputManager & InputManager::getInstance()
	{
		static InputManager manager;
		return manager;
	}

	void InputManager::init(GLFWwindow *pWindow)
	{
		window = pWindow;
	}

	bool InputManager::isKeyPressed(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

}
