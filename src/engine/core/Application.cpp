#include <core/Application.h>
#include <SceneGraph/SceneGraph.h>
#include <render/RenderBackend.h>
#include <render/MeshRendererSystem.h>
#include <SceneGraph/Transform.h>
#include <SceneGraph/WorldTransformSystem.h>
#include <render/Components.h>
#include <core/Components.h>
#include <render/ShaderResource.h>

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
		ecs::World & world = ecs::World::getInstance();

		// scenegraph
		sg::registerComponents();
		sg::registerSystems();

		// core
		world.registerComponent<core::MeshComponent>();

		// render
		world.registerComponent<render::MeshRenderer>();

		world.registerSystem<render::MeshRendererSystem>();
		ecs::Signature s;
		s.set(world.getComponentType<sg::Transform>());
		s.set(world.getComponentType<core::MeshComponent>());
		s.set(world.getComponentType<render::MeshRenderer>());
		world.setSystemSignature<render::MeshRendererSystem>(s);

		auto& backend = render::RenderBackend::getInstance();
		backend.init();

		render::ShaderResourceManager::getInstance(); // init shader resources

		pMeshRendererSystem = world.getSystem<render::MeshRendererSystem>();
		pWorldTransformSystem = world.getSystem<sg::systems::WorldTransformSystem>();

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
			backend.preFrameUpdate();
			pMeshRendererSystem->render();
			pWorldTransformSystem->update();
			runInternal();
			backend.postFrameUpdate();
		}
	}
}
