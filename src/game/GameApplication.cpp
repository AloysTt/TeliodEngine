#include <game/GameApplication.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "SceneGraph/Transform.h"
#include "core/Camera.h"
#include "core/InputManager.h"

#include <ECS/ECS.h>
#include <SceneGraph/SceneGraph.h>
#include <render/Components.h>
#include <core/Components.h>
#include <physics/Rigidbody.h>
#include <iostream>

# define EPSILON 0.001f

namespace ecs = teliod::ecs;
namespace sg = teliod::sg;
namespace render = teliod::render;
namespace core = teliod::core;
namespace physics = teliod::physics;

void GameApplication::initInternal()
{
	ecs::World & w = ecs::World::getInstance();
	sg::SceneGraph& sg = sg::SceneGraph::getInstance();

	sg::Node * groundNode = sg.getRoot()->createChild();
	ecs::Entity ground = groundNode->getEntity();
	w.addComponent<core::MeshComponent>(ground, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("ground")));
	w.addComponent<render::MeshRenderer>(ground, render::MeshRenderer(ground,
																	  render::ShaderResourceManager::getInstance().getResource("phong_textured"),
																	  render::TextureResourceManager::getInstance().getResource("concrete")
																	  ));
	w.getComponent<sg::Transform>(ground).translate(0.0f, 0.0f, 0.0f);
	{
        physics::Rigidbody rb;
        physics::RigidbodyImplVolume * impl = new physics::RigidbodyImplVolume();
        rb.setImpl(impl);
        impl->box.size = {100.0f, 0.5f, 100.0f};
        impl->box.position = {0.0f, 0.0f, 0.0f};
        impl->position = {0.0f, 0.0f, 0.0f};
        impl->orientation = {0.0f, 0.0f, 0.0f};
		impl->mass = 0.0f;
        w.addComponent<physics::Rigidbody>(ground, std::move(rb));
    }

//	sg::Node * cubeNode = sg.getRoot()->createChild();
//    cube = cubeNode->getEntity();
//	sg::Node * cube2Node = sg.getRoot()->createChild();
//	ecs::Entity cube2 = cube2Node->getEntity();
//    sg::Node * cube3Node = sg.getRoot()->createChild();
//    ecs::Entity cube3 = cube3Node->getEntity();
//
//    auto & tf1 = w.getComponent<sg::Transform>(cube);
//    auto & tf2 = w.getComponent<sg::Transform>(cube2);
//    auto & tf3 = w.getComponent<sg::Transform>(cube3);
//
//	w.addComponent<core::MeshComponent>(cube, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("cube")));
//	w.addComponent<render::MeshRenderer>(cube, render::MeshRenderer(cube, render::ShaderResourceManager::getInstance().getResource("default")));
//    {
//        physics::Rigidbody cubeRB;
//        physics::RigidbodyImplVolume * implCube = new physics::RigidbodyImplVolume();
//        cubeRB.setImpl(implCube);
//        implCube->box.size = {1.0f, 1.0f, 1.0f};
//        implCube->mass = 0.0f;
//        w.addComponent<physics::Rigidbody>(cube, std::move(cubeRB));
//    }
//
//	w.addComponent<core::MeshComponent>(cube2, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("cube")));
//	w.addComponent<render::MeshRenderer>(cube2, render::MeshRenderer(cube2, render::ShaderResourceManager::getInstance().getResource("default")));
//	w.getComponent<sg::Transform>(cube2).translate({0.0f, 5.0, -1.5});
//    {
//        physics::Rigidbody cubeRB;
//        physics::RigidbodyImplVolume * implCube = new physics::RigidbodyImplVolume();
//        cubeRB.setImpl(implCube);
//        implCube->box.size = {1.0f, 1.0f, 1.0f};
//        implCube->box.position = {0.0f, 5.0f, -1.5f};
//        implCube->position = {0.0f, 5.0f, -1.5f};
//        implCube->orientation = {0.0f, 0.0f, glm::radians(0.0f)};
//        w.addComponent<physics::Rigidbody>(cube2, std::move(cubeRB));
//    }
//
//    w.addComponent<core::MeshComponent>(cube3, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("cube")));
//    w.addComponent<render::MeshRenderer>(cube3, render::MeshRenderer(cube3, render::ShaderResourceManager::getInstance().getResource("default")));
//    w.getComponent<sg::Transform>(cube3).translate({1.0f, 10.0, 1.0});
//    {
//        physics::Rigidbody cubeRB;
//        physics::RigidbodyImplVolume * implCube = new physics::RigidbodyImplVolume();
//        cubeRB.setImpl(implCube);
//        implCube->box.size = {1.0f, 1.0f, 1.0f};
//        implCube->box.position = {1.0f, 10.0f, 1.0f};
//        implCube->position = {1.0f, 10.0f, 1.0f};
//        implCube->orientation = {0.0f, 0.0f, glm::radians(45.0f)};
//        w.addComponent<physics::Rigidbody>(cube3, std::move(cubeRB));
//    }
//    core::Camera::getInstance().getTransform().setPosition({-6.0f, 5.0f, 1.0f});

	// camera
	core::InputManager::getInstance().addMouseMovementCallback([](float offsetX, float offsetY)
	{
		sg::Transform & tf = core::Camera::getInstance().getTransform();
		constexpr float cameraSpeed = 0.05f;

		core::Camera::getInstance().addYaw(offsetX*cameraSpeed);
		core::Camera::getInstance().addPitch(-offsetY*cameraSpeed);
	});


	sg::Node * carNode = sg.getRoot()->createChild();
	car = carNode->getEntity();

	w.addComponent<core::MeshComponent>(car, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("car")));
	w.addComponent<render::MeshRenderer>(car,
										 render::MeshRenderer(
												 car,
												 render::ShaderResourceManager::getInstance().getResource("phong_textured"),
												 render::TextureResourceManager::getInstance().getResource("car")
												 )
										 );
	w.getComponent<sg::Transform>(car).translate(0.0f, 10.0f, 0.0f);
	{
		physics::Rigidbody rb;
		physics::RigidbodyImplVolume * impl = new physics::RigidbodyImplVolume();
		rb.setImpl(impl);
		impl->box.size = {2.5f, 2.0f, 5.0f};
		impl->box.position = {0.0f, 10.0f, 0.0f};
		impl->position = {0.0f, 10.0f, 0.0f};
		impl->orientation = {0.0f, 0.0f, 0.0f};
		impl->mass = 50.0f;
		w.addComponent<physics::Rigidbody>(car, std::move(rb));
	}
}

void GameApplication::destroyInternal()
{

}

void GameApplication::runInternal(float dt)
{
//	ecs::World::getInstance().getComponent<sg::Transform>(cube).rotate(glm::radians(1.0f) * dt, {0.0f, 1.0f, 0.0f});

	sg::Transform & tf = core::Camera::getInstance().getTransform();
	glm::vec3 camDir{tf.getDirection()};
	glm::vec3 inputDir{0.0f, 0.0f, 0.0f};

	constexpr float cameraSpeed = 0.2f;
	glm::vec3 camRight = glm::normalize(glm::cross(camDir, {0.0f, 1.0f, 0.0f}));

	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_W))
	{
		inputDir+=camDir;
	}
	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_S))
	{
		inputDir-=camDir;
	}
	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_D))
	{
		inputDir+=camRight;
	}
	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_A))
	{
		inputDir-=camRight;
	}

	if (inputDir.x != 0.0f || inputDir.y != 0.0f || inputDir.z != 0.0f)
		tf.translate(glm::normalize(inputDir)*dt*cameraSpeed);

	physics::RigidbodyImplVolume * pRB = static_cast<physics::RigidbodyImplVolume *>(ecs::World::getInstance().getComponent<physics::Rigidbody>(car).getImpl());
	sg::Transform & pTF = ecs::World::getInstance().getComponent<sg::Transform>(car);

	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_E))
	{
		pRB->position[1]=15.0f;
	}

	constexpr float impulseForce = 1.0f;
	constexpr float angularImpulseForce = 1.0f;

	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_UP) && glm::length2(pRB->velocity) < 150.0f)
	{
		pRB->addLinearImpulse(pTF.getDirection()*impulseForce);
	}
	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_DOWN) && glm::length2(pRB->velocity) < 150.0f)
	{
		pRB->addLinearImpulse(-pTF.getDirection()*impulseForce);
	}
	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_LEFT) && glm::length2(pRB->angVel) < 100.0f)
	{
		pRB->addRotationalImpulse(pTF.getPosition() + pTF.getDirection(), glm::cross(pTF.getDirection(), {0.0f, 1.0f, 0.0f})*angularImpulseForce*(-1.0f));
	}
	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_RIGHT) && glm::length2(pRB->angVel) < 100.0f)
	{
		pRB->addRotationalImpulse(pTF.getPosition() + pTF.getDirection(), glm::cross(pTF.getDirection(), {0.0f, 1.0f, 0.0f})*angularImpulseForce);
	}
}
