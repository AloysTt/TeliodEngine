#include <core/InputManager.h>
#include "GLFW/glfw3.h"

namespace teliod::core
{
	InputManager & InputManager::getInstance()
	{
		static InputManager manager;
		return manager;
	}

	InputManager::InputManager()
	: window(nullptr)
	, isHoveringWindow(false)
	{

	}

	void InputManager::init(GLFWwindow *pWindow)
	{
		window = pWindow;
		glfwSetKeyCallback(window, [](GLFWwindow * _window, int key, int scancode, int action, int mods){
			InputManager::getInstance().keyCallback(_window, key, scancode, action, mods);
		});
		glfwSetMouseButtonCallback(window, [](GLFWwindow * _window, int button, int action, int mods){
			InputManager::getInstance().mouseButtonCallback(_window, button, action, mods);
		});
		glfwSetCursorEnterCallback(window, [](GLFWwindow * _window, int entered){
			InputManager::getInstance().mouseEnterCallback(_window, entered);
		});
	}

	bool InputManager::isKeyPressed(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	void InputManager::captureMouse()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	void InputManager::releaseMouse()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void InputManager::keyCallback(GLFWwindow * _window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			releaseMouse();
		}
	}

	void InputManager::mouseButtonCallback(GLFWwindow * _window, int button, int action, int mods)
	{
		if (!isHoveringWindow)
			return;

		if (isHoveringWindow && button == GLFW_MOUSE_BUTTON_LEFT)
		{
			captureMouse();
		}
	}

	void InputManager::mouseEnterCallback(GLFWwindow *_window, int entered)
	{
		isHoveringWindow = entered == GLFW_TRUE;
	}
}
