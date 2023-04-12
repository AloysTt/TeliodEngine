#include <core/Application.h>
#include <SceneGraph/SceneGraph.h>
#include <render/RenderBackend.h>

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

		auto& backend = render::RenderBackend::getInstance();
		backend.init();

		initInternal();
	}

	void Application::destroy()
	{
		destroyInternal();

		auto& backend = render::RenderBackend::getInstance();
		backend.destroy();
	}

	void Application::run()
	{
		auto& backend = render::RenderBackend::getInstance();
		while (!shouldClose && !backend.windowShouldClose())
		{
			runInternal();
			backend.update();
		}
	}
}
