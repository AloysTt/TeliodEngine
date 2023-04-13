#include "render/ShaderResource.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace teliod;

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

	auto & manager = render::ShaderResourceManager::getInstance();
	auto val = manager.getResource("default");

	glfwTerminate();
}
