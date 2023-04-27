#ifndef TELIOD_CORE_INPUTMANAGER_H
#define TELIOD_CORE_INPUTMANAGER_H

class GLFWwindow;

#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>

namespace teliod::core
{
	class InputManager
	{
        static InputManager inputManager;

        //std::bitset<64> input;
        const unsigned short EnableMask = 0x8000;
        const unsigned short DisableMask = 0x7fff;
        std::unordered_map< std::string, unsigned short > action_mappings;

        void* UpdateCallback( GLFWwindow* _window, int key, int scancode, int action, int mods );

<<<<<<< Updated upstream
=======
        void MapAction( std::string& action, int key );

        bool isKeyPressed( int key ) const;
        bool isActionDone( std::string& action ) const;
>>>>>>> Stashed changes


        public:
            static InputManager & getInstance();
            void init(GLFWwindow * pWindow);
            void MapAction( std::string& action, int key );

            bool isKeyPressed( int key ) const;
            bool isActionDone( std::string& action ) const;

<<<<<<< Updated upstream
        private:
            GLFWwindow * window;
    };
=======
        const unsigned short EnableMask = 0x8000;
        const unsigned short DisableMask = 0x7fff;
        std::unordered_map< std::string, unsigned short > action_mappings;

		GLFWwindow * window;
	};
>>>>>>> Stashed changes
}

#endif // TELIOD_CORE_INPUTMANAGER_H
