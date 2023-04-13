#include <render/RenderBackend.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>

namespace teliod::render
{
	RenderBackend & RenderBackend::getInstance()
	{
		static RenderBackend instance;
		return instance;
	}

	void RenderBackend::init()
	{
		int res = glfwInit();
		assert(res != GLFW_FALSE && "Failed to initialize GLFW.");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(800, 600, "Game", nullptr, nullptr);
		if (nullptr == window)
		{
			glfwTerminate();
			assert(false && "Failed to create a window.");
		}

		glfwSetFramebufferSizeCallback(window,
		   [](GLFWwindow * w, int width, int height)
		   {
			   glViewport(0, 0, width, height);
		   }
		);

		glfwMakeContextCurrent(window);
		res = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		assert(res && "Failed to initialize GLAD.");
	}

	void RenderBackend::destroy()
	{
		glfwTerminate();
	}

	void RenderBackend::preFrameUpdate()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderBackend::postFrameUpdate()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	bool RenderBackend::windowShouldClose() const
	{
		return glfwWindowShouldClose(window);
	}
}
