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

#include <GLFW/glfw3.h>

namespace teliod::core
{
	Application::Application()
	: shouldClose(false)
	, lastPhysicsFrameTime()
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
		auto * test = world.registerSystem<core::CameraSystem>();
        core::Camera::getInstance(); // init cam
		ecs::Signature sCam;
		sCam.set(world.getComponentType<sg::Transform>());
		sCam.set(world.getComponentType<core::CameraComponent>());
		world.setSystemSignature<core::CameraSystem>(sCam);
		{
			// create camera
			ecs::Entity cam = sg::SceneGraph::getInstance().getRoot()->createChild()->getEntity();
			world.addComponent(cam, core::CameraComponent{});
            core::Camera::getInstance().entity = cam;
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

		// physics

		world.registerComponent<physics::Rigidbody>();

		world.registerSystem<physics::PhysicsSystem>();
		ecs::Signature sPhysics;
		sPhysics.set(world.getComponentType<sg::Transform>());
		sPhysics.set(world.getComponentType<physics::Rigidbody>());
		world.setSystemSignature<physics::PhysicsSystem>(sPhysics);

		pMeshRendererSystem = world.getSystem<render::MeshRendererSystem>();
		pWorldTransformSystem = world.getSystem<sg::systems::WorldTransformSystem>();
		pCameraSystem = world.getSystem<core::CameraSystem>();
		pPhysicsSystem = world.getSystem<physics::PhysicsSystem>();

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
		double lastFrame = glfwGetTime();
		while (!shouldClose && !backend.windowShouldClose())
		{
			backend.preFrameUpdate();
			pMeshRendererSystem->render();
			pWorldTransformSystem->update();
			pCameraSystem->update();

			double diff = glfwGetTime() - lastFrame;
			lastFrame = glfwGetTime();
			runInternal(diff*100.0f); // centiseconds

			double diffPhysics = glfwGetTime() - lastPhysicsFrameTime;
			if (diffPhysics > 1.0f/30.0f)
			{
				lastPhysicsFrameTime = glfwGetTime();
				pPhysicsSystem->update(diffPhysics);
			}
			backend.postFrameUpdate();
		}
	}
}
