#include <core/InputManager.h>
#include "GLFW/glfw3.h"

namespace teliod::core
{
	InputManager & InputManager::getInstance()
    {
        static InputManager inputManager;
        return inputManager;
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

        std::string LOL("LOL");
        std::string LMAO("LMAO");
        InputManager::getInstance().MapAction(LOL, GLFW_KEY_F1 );
        InputManager::getInstance().MapAction(LMAO, GLFW_KEY_F2 );
	}

    void InputManager::MapAction( std::string& action, int key )
    {
        if( action.length() == 0 || key == GLFW_KEY_UNKNOWN )
            return;

        action_mappings[action] = key;
    }

    bool InputManager::isKeyPressed(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

    bool InputManager::isActionDone(std::string& action) const
    {
        return action_mappings.at(action)&EnableMask;
    }

	void InputManager::captureMouse()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		isCursorCaptured = true;
	}

	void InputManager::keyCallback(GLFWwindow * _window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            releaseMouse();

        for( auto& ActionKey : action_mappings )
        {
            if (key == (ActionKey.second & DisableMask))
            {
                if ( action == GLFW_PRESS )
                    ActionKey.second |= EnableMask;
                else if ( action == GLFW_RELEASE )
                    ActionKey.second &= DisableMask;
            }

            std::string a(ActionKey.first);
            std::cout << ActionKey.first << " pressed ? " << isActionDone(a) << '\n';
        }
    }

	void InputManager::releaseMouse()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		isCursorCaptured = false;
	}

	void InputManager::mouseButtonCallback(GLFWwindow * _window, int button, int action, int mods)
	{
		if (!isHoveringWindow)
			return;

		if ( button == GLFW_MOUSE_BUTTON_LEFT)
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
