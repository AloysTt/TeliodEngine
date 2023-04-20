#ifndef TELIOD_CORE_CAMERA_H
#define TELIOD_CORE_CAMERA_H

#include <ECS/ECS.h>
#include <glm/mat4x4.hpp>

namespace teliod::sg
{
	class Transform;
}

namespace teliod::core
{
	// component
	class CameraComponent
	{
	};

	class CameraSystem : public ecs::System
	{
	public:
		void update();
	};

	class Camera
	{
		friend class CameraSystem;
	public:
		static Camera & getInstance();

		const glm::mat4 & getViewMatrix() const;
		const glm::mat4 & getProjMatrix() const;

		sg::Transform & getTransform();

		~Camera() = default;
	private:
		Camera();

		glm::mat4x4 view;
		glm::mat4x4 proj;
		ecs::Entity entity;
	};
}

#endif // TELIOD_CORE_CAMERA_H
