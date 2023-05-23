#include <core/Camera.h>
#include "SceneGraph/Transform.h"
#define EPSILON 1e-05f

namespace teliod::core
{
	void CameraSystem::update()
	{
		Camera & cam = Camera::getInstance();
		cam.entity = *getEntities().begin();
		sg::Transform & tf = ecs::World::getInstance().getComponent<sg::Transform>(cam.entity);
		auto & test = ecs::World::getInstance().getComponent<core::CameraComponent>(cam.entity);

		glm::vec3 dir;
		dir.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
		dir.y = sin(glm::radians(cam.pitch));
		dir.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
		dir = glm::normalize(dir);

		glm::quat dirQuat{glm::quatLookAtLH(dir, {0.0f, 1.0f, 0.0f})};
		tf.setRotation(dirQuat);
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
	, pitch(0)
	, yaw(0)
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

	void Camera::addPitch(float _pitch)
	{
		pitch+=_pitch;
		pitch = glm::clamp(pitch, -90.0f+EPSILON, 90.0f-EPSILON);
	}

	void Camera::addYaw(float _yaw)
	{
		yaw+=_yaw;
		if (yaw < 0.0f) yaw +=360.0f;
		if (yaw > 360.0f) yaw -=360.0f;
	}
}
