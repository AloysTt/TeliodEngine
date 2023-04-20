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
	private:
		GLFWwindow * window;
	};
}

#endif // TELIOD_CORE_INPUTMANAGER_H
