#include <core/Application.h>
#include <SceneGraph/SceneGraph.h>

namespace teliod::core
{

	Application::Application()
	: shouldClose(false)
	{
	}

	Application::~Application()
	{
	}

	void Application::init()
	{
		sg::registerComponents();
		sg::registerSystems();

		initInternal();
	}

	void Application::destroy()
	{
		destroyInternal();
	}

	void Application::run()
	{
		while (!shouldClose)
		{
			runInternal();
		}
	}
}
