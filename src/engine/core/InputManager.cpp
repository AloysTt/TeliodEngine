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
			, isCursorCaptured(false)
			, lastPosX(0.0f)
			, lastPosY(0.0f)
			, mouseMovementCallbacks()
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
		glfwSetCursorPosCallback(window, [](GLFWwindow * _window, double xpos, double ypos){
			InputManager::getInstance().cursorPosCallback(_window, xpos, ypos);
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
		isCursorCaptured = true;
	}

	void InputManager::releaseMouse()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		isCursorCaptured = false;
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

	void InputManager::mouseEnterCallback(GLFWwindow * _window, int entered)
	{
		isHoveringWindow = entered == GLFW_TRUE;
	}

	void InputManager::cursorPosCallback(GLFWwindow * _window, double xpos, double ypos)
	{
		float offsetX = xpos - lastPosX;
		float offsetY = ypos - lastPosY;

		if (isCursorCaptured)
		{
			for (auto & func : mouseMovementCallbacks)
			{
				func(offsetX, offsetY);
			}
		}

		lastPosX = xpos;
		lastPosY = ypos;
	}

	void InputManager::addMouseMovementCallback(const std::function<void(float, float)> & callback)
	{
		mouseMovementCallbacks.push_back(callback);
	}
}
