#include <core/Camera.h>
#include "SceneGraph/Transform.h"

namespace teliod::core
{
	void CameraSystem::update()
	{
		Camera & cam = Camera::getInstance();
		cam.entity = *getEntities().begin();
		const sg::Transform & tf = ecs::World::getInstance().getComponent<sg::Transform>(cam.entity);
		glm::vec3 dir{glm::rotate(glm::inverse(tf.getRotation()), glm::vec3(0.0, 0.0, -1.0))};

		cam.view = glm::lookAt(tf.getPosition(), tf.getPosition()+dir, {0.0f, 1.0f, 0.0f});
	}

	Camera & Camera::getInstance()
	{
		static Camera camera;
		return camera;
	}

	Camera::Camera()
	: view(1.0f)
	, proj(glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f))
	, entity(0)
	{
	}

	const glm::mat4 & Camera::getViewMatrix() const
	{
		return view;
	}

	const glm::mat4 & Camera::getProjMatrix() const
	{
		return proj;
	}

	sg::Transform & Camera::getTransform()
	{
		return ecs::World::getInstance().getComponent<sg::Transform>(entity);
	}
}
