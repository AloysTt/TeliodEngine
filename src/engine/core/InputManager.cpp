#include <core/InputManager.h>
#include "GLFW/glfw3.h"

namespace teliod::core
{
	InputManager & InputManager::getInstance()
	{
		return inputManager;
	}

    void InputManager::init(GLFWwindow *pWindow)
    {
        window = pWindow;

        glfwSetKeyCallback( window, UpdateCallback );
    }

    void* InputManager::UpdateCallback( GLFWwindow* _window, int key, int scancode, int action, int mods )
    {
        for( auto& ActionKey : action_mappings )
        {
            if( key == (ActionKey.second&DisableMask) && action == GLFW_PRESS )
                ActionKey.second |= EnableMask;
            else
                ActionKey.second &= DisableMask;
        }
    }
    
    void InputManager::MapAction( std::string& action, int key )
    {
        if( action.length() == 0 || key == GLFW_KEY_UNKNOWN )
            return;

        action_mappings[action] = key;

        UpdateCallback( action, key );
    }

	bool InputManager::isKeyPressed(int key) const
	{
		return glfwGetKey( window, key ) == GLFW_PRESS;
	}

    bool InputManager::isActionDone(std::string& action) const
    {
        return action_mappings.at(action)&EnableMask;
    }

}
