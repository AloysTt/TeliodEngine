#ifndef TELIOD_CORE_INPUTMANAGER_H
#define TELIOD_CORE_INPUTMANAGER_H

class GLFWwindow;

namespace teliod::core
{
	class InputManager
	{
	public:
		static InputManager & getInstance();
		void init(GLFWwindow * pWindow);

		bool isKeyPressed(int key) const;

		void captureMouse();
		void releaseMouse();

		~InputManager() = default;
	private:
		InputManager();
		void keyCallback(GLFWwindow * _window, int key, int scancode, int action, int mods);
		void mouseButtonCallback(GLFWwindow * _window, int button, int action, int mods);
		void mouseEnterCallback(GLFWwindow * _window, int entered);

		// state
		bool isHoveringWindow;
		bool isCursorCaptured;

		GLFWwindow * window;
	};
}

#endif // TELIOD_CORE_INPUTMANAGER_H
