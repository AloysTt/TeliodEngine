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
#include <cmath>
#include <random>

# define EPSILON 0.001f

namespace ecs = teliod::ecs;
namespace sg = teliod::sg;
namespace render = teliod::render;
namespace core = teliod::core;
namespace physics = teliod::physics;

void GameApplication::initInternal()
{
	collided = false;
	timeSinceCollided = 0.0f;
	score = 0;

	ecs::World & w = ecs::World::getInstance();
	sg::SceneGraph& sg = sg::SceneGraph::getInstance();

	sg::Node * groundNode = sg.getRoot()->createChild();
	ecs::Entity ground = groundNode->getEntity();
	sg::Node * groundNode2 = sg.getRoot()->createChild();
	ecs::Entity ground2 = groundNode2->getEntity();

	w.addComponent<core::MeshComponent>(ground, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("ground")));
	w.addComponent<render::MeshRenderer>(ground, render::MeshRenderer(ground,
																	  render::ShaderResourceManager::getInstance().getResource("phong_textured"),
																	  render::TextureResourceManager::getInstance().getResource("concrete")
																	  ));
	w.getComponent<sg::Transform>(ground2).translate(0.0f, -10.0f, 0.0f);
	{
        physics::Rigidbody rb;
        physics::RigidbodyImplVolume * impl = new physics::RigidbodyImplVolume();
        rb.setImpl(impl);
        impl->box.size = {1000.0f, 10.5f, 1000.0f};
        impl->box.position = {0.0f, -10.0f, 0.0f};
        impl->position = {0.0f, -10.0f, 0.0f};
        impl->orientation = {0.0f, 0.0f, 0.0f};
		impl->mass = 0.0f;
        w.addComponent<physics::Rigidbody>(ground2, std::move(rb));
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
//	core::InputManager::getInstance().addMouseMovementCallback([](float offsetX, float offsetY)
//	{
//		sg::Transform & tf = core::Camera::getInstance().getTransform();
//		constexpr float cameraSpeed = 0.05f;
//
//		core::Camera::getInstance().addYaw(offsetX*cameraSpeed);
//		core::Camera::getInstance().addPitch(-offsetY*cameraSpeed);
//	});


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
		impl->box.size = {4.5f, 2.0f, 5.0f};
		impl->box.position = {0.0f, 10.0f, 0.0f};
		impl->position = {0.0f, 10.0f, 0.0f};
		impl->orientation = {0.0f, 0.0f, 0.0f};
		impl->mass = 5.0f;
		w.addComponent<physics::Rigidbody>(car, std::move(rb));
	}

	// goal
	sg::Node * goalNode = sg.getRoot()->createChild();
	goal = goalNode->getEntity();


	w.addComponent<core::MeshComponent>(goal, core::MeshComponent(core::MeshResourceManager::getInstance().getResource("tower")));
	w.addComponent<render::MeshRenderer>(goal, render::MeshRenderer(goal,
																	  render::ShaderResourceManager::getInstance().getResource("phong_textured"),
																	  render::TextureResourceManager::getInstance().getResource("blue")
	));
	w.getComponent<sg::Transform>(goal).translate(0.0f, 12.0f, 15.0f);
	{
		physics::Rigidbody rb;
		physics::RigidbodyImplVolume * impl = new physics::RigidbodyImplVolume();
		rb.setImpl(impl);
		impl->box.size = {1.0f, 10.0f, 1.0f};
		impl->box.position = {0.0f, 12.0f, 15.0f};
		impl->position = {0.0f, 12.0f, 15.0f};
		impl->orientation = {0.0f, 0.0f, 0.0f};
		impl->mass = 1.0f;
		w.addComponent<physics::Rigidbody>(goal, std::move(rb));
	}

	ecs::Entity tmp = goal;
	w.getSystem<physics::PhysicsSystem>()->addCallback(car, [this, tmp](ecs::Entity ent){
		if (!collided && ent == tmp)
		{
			collided = true;
		}
	});
}

void GameApplication::destroyInternal()
{

}

void GameApplication::runInternal(float dt)
{
//	ecs::World::getInstance().getComponent<sg::Transform>(cube).rotate(glm::radians(1.0f) * dt, {0.0f, 1.0f, 0.0f});

	core::Camera & camera = core::Camera::getInstance();
	sg::Transform & tfCam = camera.getTransform();
	sg::Transform & tfCar = ecs::World::getInstance().getComponent<sg::Transform>(car);
	glm::vec3 camDir{tfCam.getDirection()};
	glm::vec3 inputDir{0.0f, 0.0f, 0.0f};

//	constexpr float cameraSpeed = 0.2f;
//	glm::vec3 camRight = glm::normalize(glm::cross(camDir, {0.0f, 1.0f, 0.0f}));
//
//	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_W))
//	{
//		inputDir+=camDir;
//	}
//	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_S))
//	{
//		inputDir-=camDir;
//	}
//	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_D))
//	{
//		inputDir+=camRight;
//	}
//	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_A))
//	{
//		inputDir-=camRight;
//	}
//
//	if (inputDir.x != 0.0f || inputDir.y != 0.0f || inputDir.z != 0.0f)
//		tf.translate(glm::normalize(inputDir)*dt*cameraSpeed);

	glm::vec3 carDir = tfCar.getDirection();
	carDir.y = 0.0f;
	carDir = glm::normalize(carDir);

	glm::vec3 newPos{tfCar.getPosition()};
	newPos-=carDir*15.0f;
	newPos.y+= 5.0f;
	tfCam.setPosition(newPos);
	glm::vec3 camTarget = tfCar.getPosition();
	camTarget.y += 1.0f;
	glm::vec3 dir = glm::normalize(camTarget-newPos);
	camera.setYaw(glm::degrees(atan2(dir.z, dir.x)));
	camera.setPitch(glm::degrees(atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z))));
	// glm::quatLookAtLH(()
//	tfCam.setRotation(, {0.0f, 1.0f, 0.0f}));



	physics::RigidbodyImplVolume * pRB = static_cast<physics::RigidbodyImplVolume *>(ecs::World::getInstance().getComponent<physics::Rigidbody>(car).getImpl());
	sg::Transform & pTF = ecs::World::getInstance().getComponent<sg::Transform>(car);
	if (fabsf(pRB->orientation.z) > 0.01f)
		pRB->orientation.z *= 0.8f;
	if (fabsf(pRB->angVel.z) > 0.01f)
		pRB->angVel.z *= 0.8f;
	if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_E))
	{
		pRB->position[1]=15.0f;
	}

	constexpr float impulseForce = 0.01f;
	constexpr float angularImpulseForce = 0.05f;

	glm::vec3 carUp = glm::rotate(tfCar.getRotation(), glm::vec3(0.0, 1.0, 0.0));
	carDir = tfCar.getDirection();
	glm::vec3 side = glm::cross(carDir, carUp);

	if (tfCar.getPosition().y < 2.6f)
	{
		if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_UP) && glm::length(pRB->velocity) < 50.0f)
		{
			pRB->addLinearImpulse(pTF.getDirection() * impulseForce);
		}
		if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_DOWN) && glm::length(pRB->velocity) < 50.0f)
		{
			pRB->addLinearImpulse(-pTF.getDirection() * impulseForce);
		}
		if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_LEFT) && glm::length(pRB->angVel) < 5.0f)
		{
			pRB->addRotationalImpulse(pTF.getPosition() + pTF.getDirection(), side*angularImpulseForce*(-1.0f));
		}
		if (core::InputManager::getInstance().isKeyPressed(GLFW_KEY_RIGHT) && glm::length(pRB->angVel) < 5.0f)
		{
			pRB->addRotationalImpulse(pTF.getPosition() + pTF.getDirection(), side*angularImpulseForce);
		}
	}

	if (collided)
	{
		timeSinceCollided+=dt;
		if (timeSinceCollided > 200.0f)
		{
			ecs::World & w = ecs::World::getInstance();
			collided = false;
			timeSinceCollided = 0.0f;
			w.removeComponent<physics::Rigidbody>(goal);
			sg::Transform & goalTF = w.getComponent<sg::Transform>(goal);

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.0, 50.0);
			glm::vec3 newPos{dis(gen), 12.0f, dis(gen)};



			goalTF.setPosition(newPos);
			goalTF.setRotation({1.0f, 0.0f, 0.0f, 0.0f});
			{
				physics::Rigidbody rb;
				physics::RigidbodyImplVolume * impl = new physics::RigidbodyImplVolume();
				rb.setImpl(impl);
				impl->box.size = {1.0f, 10.0f, 1.0f};
				impl->box.position = newPos;
				impl->position = newPos;
				impl->orientation = {0.0f, 0.0f, 0.0f};
				impl->mass = 1.0f;
				w.addComponent<physics::Rigidbody>(goal, std::move(rb));
			}
			++score;
			std::cout << "Score : " << score << std::endl;
		}
	}
}
