#include "glad/glad.h"
#include "GLFW/glfw3.h"


int main(int argc, char **argv)
{
	if (glfwInit() == GLFW_FALSE)
		return -1;

	GLFWwindow *window = glfwCreateWindow(800, 600, "Ouais", nullptr, nullptr);
	if (nullptr == window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}