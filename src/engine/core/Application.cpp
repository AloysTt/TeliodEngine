#include <core/Application.h>
#include <SceneGraph/SceneGraph.h>
#include <render/RenderBackend.h>
#include <render/MeshRendererSystem.h>
#include <SceneGraph/Transform.h>
#include <SceneGraph/WorldTransformSystem.h>
#include <render/Components.h>
#include <core/Components.h>
#include <render/ShaderResource.h>
#include <core/Camera.h>
#include <chrono>
#include "core/InputManager.h"

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
		world.registerComponent<core::CameraComponent>();
		world.registerSystem<core::CameraSystem>();
		ecs::Signature sCam;
		sCam.set(world.getComponentType<sg::Transform>());
		sCam.set(world.getComponentType<core::CameraComponent>());
		world.setSystemSignature<core::CameraSystem>(sCam);
		{
			// create camera
			ecs::Entity cam = sg::SceneGraph::getInstance().getRoot()->createChild()->getEntity();
			world.addComponent(cam, core::CameraComponent{});
		}

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
		core::InputManager::getInstance().init(backend.getWindow());

		render::ShaderResourceManager::getInstance(); // init shader resources

		pMeshRendererSystem = world.getSystem<render::MeshRendererSystem>();
		pWorldTransformSystem = world.getSystem<sg::systems::WorldTransformSystem>();
		pCameraSystem = world.getSystem<core::CameraSystem>();

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
		auto lastFrame = std::chrono::high_resolution_clock::now();
		while (!shouldClose && !backend.windowShouldClose())
		{
			backend.preFrameUpdate();
			pMeshRendererSystem->render();
			pWorldTransformSystem->update();
			pCameraSystem->update();

			std::chrono::duration<float, std::centi> diff(std::chrono::high_resolution_clock::now() - lastFrame);
			lastFrame = std::chrono::high_resolution_clock::now();
			runInternal(diff.count());

			backend.postFrameUpdate();
		}
	}
}
