#include <game/GameApplication.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


void GameApplication::initInternal()
{

}

void GameApplication::destroyInternal()
{

}

void GameApplication::runInternal()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}
