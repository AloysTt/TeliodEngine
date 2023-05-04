#ifndef TELIOD_CORE_INPUTMANAGER_H
#define TELIOD_CORE_INPUTMANAGER_H

#include <functional>

class GLFWwindow;

#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>

namespace teliod::core
{
	class InputManager
	{
	public:
		static InputManager & getInstance();
		void init(GLFWwindow * pWindow);
		void MapAction( std::string& action, int key );
		bool isKeyPressed( int key ) const;
        bool isActionDone( std::string& action ) const;

		void captureMouse();
		void releaseMouse();

		void addMouseMovementCallback(const std::function<void(float, float)> & callback);

		~InputManager() = default;
	private:
		InputManager();
		void keyCallback(GLFWwindow * _window, int key, int scancode, int action, int mods);
		void mouseButtonCallback(GLFWwindow * _window, int button, int action, int mods);
		void mouseEnterCallback(GLFWwindow * _window, int entered);
		void cursorPosCallback(GLFWwindow * _window, double xpos, double ypos);

		// callbacks
		std::vector<std::function<void(float, float)>> mouseMovementCallbacks;

		// state
		bool isHoveringWindow;
		bool isCursorCaptured;
		float lastPosX;
		float lastPosY;

		GLFWwindow * window;
        const unsigned short EnableMask = 0x8000;
        const unsigned short DisableMask = 0x7fff;
        std::unordered_map< std::string, unsigned short > action_mappings;
	};
}

#endif // TELIOD_CORE_INPUTMANAGER_H
